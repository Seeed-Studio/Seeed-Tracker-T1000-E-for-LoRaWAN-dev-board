
#include "app_timer.h"
#include "smtc_hal.h"
#include "app_led.h"
#include "app_beep.h"
#include "app_button.h"
#include "smtc_modem_api.h"

APP_TIMER_DEF(m_button_event_timer_id);
APP_TIMER_DEF(m_ble_adv_event_timer_id);
APP_TIMER_DEF(m_sos_cnt_event_timer_id);
APP_TIMER_DEF(m_test_mode_event_timer_id);

static bool button_event = false;
static uint32_t button_rise_time = 0;
static uint32_t button_fall_time = 0;

static bool button_long_det = false;
static uint8_t button_click_cnt = 0;

static bool ble_adv_flag = false;

static bool sos_status = 0;
static uint8_t sos_count = 0;

uint8_t button_sos_type = 1;
bool sos_in_progress = false;

bool button_power = true;
bool new_power_off = false;

void app_button_irq_handler( void *obj );

hal_gpio_irq_t app_button_irq = {
    .pin = USER_BUTTON,
    .callback = app_button_irq_handler,
    .context = NULL,
};

extern uint8_t tracker_test_mode;
extern uint8_t app_beep_state;
extern uint8_t app_led_state;

void app_button_irq_handler( void *obj )
{
    button_event = true;
    if( hal_gpio_get_value( USER_BUTTON ))
    {
        button_rise_time = hal_rtc_get_time_ms( );
    }
    else
    {
        button_fall_time = hal_rtc_get_time_ms( );
    }
}

void app_user_button_event_timeout_handler( void )
{
    if( button_long_det )
    {
        if(( hal_gpio_get_value( USER_BUTTON ) == 1 ) && ( hal_rtc_get_time_ms( ) - button_rise_time ) > BUTTON_PERSS_LONG )
        {
            button_fall_time = 0;
            button_rise_time = 0;
            app_timer_stop( m_button_event_timer_id );
            // PRINTF( "\r\nBUTTON_PRESS_LONG\r\n\r\n" );
            if( button_power ) // power off
            {
                button_power = false;
                new_power_off = true;
                app_beep_power_off( );
            }
            else // power on
            {
                button_power = true;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
                while( hal_gpio_get_value( USER_BUTTON ) == 1 ) // Wait for button release, otherwise enter bootloader
                {
                    hal_gpio_init_out( USER_LED_R, HAL_GPIO_RESET );
                }
                hal_mcu_reset( );
            }
        }
        else
        {
            if( hal_gpio_get_value( USER_BUTTON ) == 1 )
            {
                app_timer_stop( m_button_event_timer_id );
                app_timer_start( m_button_event_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
            }
        }
    }
    else
    {
        if( button_power == false ) // on power off state
        {
            return;
        }

        // PRINTF( "BUTTON_PRESS_CLICK: %d\r\n", button_click_cnt );
        switch( button_click_cnt )
        {
            case BUTTON_PRESS_ONECE: // confirm alarm
            {
                button_click_cnt = 0;

                if( ble_adv_flag == true )  // skip it when on ble adv mode
                {
                    PRINTF( "BLE_ADV, SKIP_IT\r\n" );
                    return;
                }

                smtc_modem_status_mask_t modem_status;
                smtc_modem_get_status( 0, &modem_status );
                if(( modem_status & SMTC_MODEM_STATUS_JOINING ) == SMTC_MODEM_STATUS_JOINING )
                {
                    PRINTF( "LORA_JOINING, SKIP_IT\r\n" );
                    return;
                }

                if( sos_in_progress ) // stop SOS mode
                {
                    sos_in_progress = false;
                    sos_status = false;
                    sos_count = 0;
                    app_timer_stop( m_sos_cnt_event_timer_id );
                }

                app_beep_idle( );
                app_led_idle( );

                app_tracker_new_run( TRACKER_STATE_BIT8_USER );
                app_led_sos_confirm( );
                PRINTF( "\r\nCONFIRM_ALARM\r\n\r\n" );
            }
            break;

            case BUTTON_PRESS_TWICE: // SOS alarm
            {
                button_click_cnt = 0;

                if( ble_adv_flag == true )  // skip it when on ble adv mode
                {
                    PRINTF( "BLE_ADV, SKIP_IT\r\n" );
                    return;
                }

                smtc_modem_status_mask_t modem_status;
                smtc_modem_get_status( 0, &modem_status );
                if(( modem_status & SMTC_MODEM_STATUS_JOINING ) == SMTC_MODEM_STATUS_JOINING )
                {
                    PRINTF( "LORA_JOINING, SKIP_IT\r\n" );
                    return;
                }

                if( button_sos_type == 0 ) // single
                {
                    app_tracker_new_run( TRACKER_STATE_BIT7_SOS );
                    app_beep_sos( );
                    app_led_sos_run( );
                    PRINTF( "\r\nSOS_SINGLE\r\n\r\n" );
                }
                else if( button_sos_type == 1 ) // continuous
                {
                    if( sos_status == false )
                    {
                        sos_status = true;
                        sos_in_progress = true;
                        app_timer_stop( m_sos_cnt_event_timer_id );
                        app_timer_start( m_sos_cnt_event_timer_id,  APP_TIMER_TICKS( 60000 ), NULL );
                        app_tracker_new_run( TRACKER_STATE_BIT7_SOS );
                        app_beep_sos( );
                        app_led_sos_run( );
                        PRINTF( "\r\nSOS_ENTER\r\n\r\n" );
                    }
                    else
                    {
                        sos_in_progress = false;
                        sos_status = false;
                        sos_count = 0;
                        app_beep_idle( );
                        app_led_idle( );
                        app_timer_stop( m_sos_cnt_event_timer_id );
                        PRINTF( "\r\nSOS_EXIT\r\n\r\n" );
                    }
                }
            }
            break;

            case BUTTON_PRESS_THRICE: // BLE advertising start/stop
            {
                button_click_cnt = 0;
                if( ble_adv_flag == false )
                {
                    ble_adv_flag = true;
                    
                    smtc_modem_status_mask_t modem_status;
                    smtc_modem_get_status( 0, &modem_status );
                    if(( modem_status & SMTC_MODEM_STATUS_JOINING ) == SMTC_MODEM_STATUS_JOINING )
                    {
                        app_led_breathe_stop( );
                    }

                    if( sos_in_progress ) // stop SOS mode
                    {
                        sos_in_progress = false;
                        sos_status = false;
                        sos_count = 0;
                        app_timer_stop( m_sos_cnt_event_timer_id );
                    }

                    app_beep_idle( );
                    app_led_idle( );

                    app_led_ble_cfg( );
                    app_ble_advertising_start( );
                    app_timer_stop( m_ble_adv_event_timer_id );
                    app_timer_start( m_ble_adv_event_timer_id,  APP_TIMER_TICKS( 30000 ), NULL );
                    PRINTF( "\r\nBLE_ADV_START\r\n\r\n" );
                }
                else
                {
                    ble_adv_flag = false;
                    app_led_idle( );
                    app_ble_advertising_stop( );
                    app_timer_stop( m_ble_adv_event_timer_id );
                    PRINTF( "\r\nBLE_ADV_STOP\r\n\r\n" );

                    smtc_modem_status_mask_t modem_status;
                    smtc_modem_get_status( 0, &modem_status );
                    if(( modem_status & SMTC_MODEM_STATUS_JOINING ) == SMTC_MODEM_STATUS_JOINING )
                    {
                        app_led_breathe_start( );
                    }
                }
            }
            break;

            default:
            break;
        }
    }
}

void app_user_ble_adv_event_timeout_handler( void )
{
    if( app_ble_is_disconnected( ))
    {
        if( ble_adv_flag )
        {
            ble_adv_flag = false;
            app_led_idle( );
            app_ble_advertising_stop( );
            PRINTF( "\r\nBLE_ADV_TIMEOUT\r\n\r\n" );

            smtc_modem_status_mask_t modem_status;
            smtc_modem_get_status( 0, &modem_status );
            if(( modem_status & SMTC_MODEM_STATUS_JOINING ) == SMTC_MODEM_STATUS_JOINING )
            {
                app_led_breathe_start( );
            }
        }
    }
}

void app_user_sos_cnt_event_timeout_handler( void )
{
    if( sos_status )
    {
        sos_count ++;
        if( sos_count >= APP_USER_SOS_NUM_MAX )
        {
            sos_in_progress = false;
            sos_status = false;
            sos_count = 0;
            app_timer_stop( m_sos_cnt_event_timer_id );
            app_beep_idle( );
            app_led_idle( );
            PRINTF( "\r\nSOS_EXIT\r\n\r\n" );
        }
        else
        {
            app_timer_start( m_sos_cnt_event_timer_id,  APP_TIMER_TICKS( 60000 ), NULL );
            app_tracker_new_run( TRACKER_STATE_BIT7_SOS );
        }
    }
}

void app_user_test_mode_event_timeout_handler( void )
{
    if(( app_beep_state == APP_BEEP_IDLE ) && ( app_led_state == APP_LED_IDLE ))
    {
        if( button_power )
        {
            app_beep_pos_s( );
            app_led_sos_confirm( );
        }
    }
}

void app_user_button_init( void )
{
    hal_gpio_init_in( USER_BUTTON, HAL_GPIO_PULL_MODE_DOWN, HAL_GPIO_IRQ_MODE_RISING_FALLING, &app_button_irq );
    app_timer_create( &m_button_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_button_event_timeout_handler );
    app_timer_create( &m_ble_adv_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_ble_adv_event_timeout_handler );
    app_timer_create( &m_sos_cnt_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_sos_cnt_event_timeout_handler );
    app_timer_create( &m_test_mode_event_timer_id, APP_TIMER_MODE_REPEATED, app_user_test_mode_event_timeout_handler );
    if( tracker_test_mode )
    {
        app_timer_start( m_test_mode_event_timer_id, APP_TIMER_TICKS( 15000 ), NULL );
    }
}

void app_user_button_det( void )
{
    if( button_event )
    {
        button_event = false;
        // PRINTF( "%u - %u\r\n", button_fall_time, button_rise_time );
        if( hal_gpio_get_value( USER_BUTTON ) == 1 )
        {
            button_long_det = true;
            app_timer_stop( m_button_event_timer_id );
            app_timer_start( m_button_event_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
        }
        else if(( hal_gpio_get_value( USER_BUTTON ) == 0 ) && ( button_fall_time - button_rise_time ) < BUTTON_PRESS_CLICK )
        {
            button_fall_time = 0;
            button_rise_time = 0;
            button_long_det = false;
            
            app_timer_stop( m_button_event_timer_id );
            app_timer_start( m_button_event_timer_id,  APP_TIMER_TICKS( BUTTON_PRESS_CLICK ), NULL );

            button_click_cnt ++;
            if( button_click_cnt > 3 )
            {
                button_click_cnt = 3;
            }
        }
    }

    if( new_power_off )
    {
        new_power_off = false;
        app_user_power_off( );
    }
}

void app_sos_continuous_toggle_on( void )
{
    sos_status = true;
    sos_in_progress = true;
    sos_count = 0;
    app_timer_stop( m_sos_cnt_event_timer_id );
    app_timer_start( m_sos_cnt_event_timer_id,  APP_TIMER_TICKS( 60000 ), NULL );
    app_tracker_new_run( TRACKER_STATE_BIT7_SOS );
    app_beep_sos( );
    app_led_sos_run( );
    PRINTF( "\r\nSOS_ENTER\r\n\r\n" );
}

void app_sos_continuous_toggle_off( void )
{
    sos_in_progress = false;
    sos_status = false;
    sos_count = 0;
    app_beep_idle( );
    app_led_idle( );
    app_timer_stop( m_sos_cnt_event_timer_id );
    PRINTF( "\r\nSOS_EXIT\r\n\r\n" );
}

void app_user_power_off( void )
{
    // stop lbm
    app_lora_stack_suspend( );
    hal_mcu_wait_ms( 1000 );
    app_radio_set_sleep( );

    app_timer_stop( m_ble_adv_event_timer_id );
    app_timer_stop( m_sos_cnt_event_timer_id );
    app_timer_stop( m_test_mode_event_timer_id );

    app_led_breathe_stop( );
    app_led_idle( );
    app_beep_idle( );
    hal_mcu_wait_ms( 1000 );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
    hal_gpio_init_out( USER_LED_R, HAL_GPIO_RESET );

    hal_spi_deinit( );
    hal_uart_0_deinit( );
    hal_i2c_deinit( );

    // power off GPS
    hal_gpio_init_out( AG3335_POWER_EN, HAL_GPIO_RESET );
    hal_gpio_init_out( AG3335_VRTC_EN, HAL_GPIO_RESET );
    hal_gpio_init_out( AG3335_RESET, HAL_GPIO_RESET );
    hal_gpio_init_in( AG3335_SLEEP_INT, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    hal_gpio_init_in( AG3335_RTC_INT, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    hal_gpio_init_in( AG3335_RESETB_OUT, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );
    
    // power off acc
    hal_gpio_init_out( ACC_POWER, HAL_GPIO_RESET );
    hal_gpio_init_in( ACC_INT1, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_OFF, NULL );

    // power off temp&light/battery
    hal_gpio_init_out( SENSE_POWER_EN, HAL_GPIO_RESET );

    // stop ble
    ble_scan_stop( );
    app_ble_disconnect( );
    app_ble_advertising_stop( );
    hal_mcu_wait_ms( 1000 );

    PRINTF( "\r\nPOWER_OFF\r\n" );
}
