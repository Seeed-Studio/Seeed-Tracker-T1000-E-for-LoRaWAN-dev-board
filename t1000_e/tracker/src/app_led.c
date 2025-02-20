
#include "nrf_drv_pwm.h"
#include "app_timer.h"
#include "smtc_hal.h"
#include "app_button.h"
#include "app_led.h"

APP_TIMER_DEF(m_led_event_timer_id);
APP_TIMER_DEF(m_bat_event_timer_id);

static nrf_drv_pwm_t PWM2 = NRF_DRV_PWM_INSTANCE( 2 );

static uint16_t const m_pwm_top = 5120;

static nrf_pwm_values_individual_t m_pwm_seq_values;

static nrf_pwm_sequence_t const m_pwm_seq =
{
    .values.p_individual = &m_pwm_seq_values,
    .length              = NRF_PWM_VALUES_LENGTH( m_pwm_seq_values ),
    .repeats             = 0,
    .end_delay           = 0
};

static bool pwm_init = false;

static uint16_t indexWave[] = 
{
    0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 89, 98,
    107, 116, 125, 133, 142, 151, 159, 168, 176,
    184, 193, 201, 209, 218, 226, 234, 242, 249,
    257, 265, 273, 280, 288, 295, 302, 310, 317,
    324, 331, 337, 344, 351, 357, 364, 370, 376,
    382, 388, 394, 399, 405, 410, 416, 421, 426,
    431, 436, 440, 445, 449, 454, 458, 462, 465,
    469, 473, 476, 479, 482, 485, 488, 491, 493,
    496, 498, 500, 502, 503, 505, 506, 508, 509,
    510, 510, 511, 512, 512, 512, 512, 512, 512,
    511, 510, 510, 509, 508, 506, 505, 503, 502,
    500, 498, 496, 493, 491, 488, 485, 482, 479,
    476, 473, 469, 465, 462, 458, 454, 449, 445,
    440, 436, 431, 426, 421, 416, 410, 405, 399,
    394, 388, 382, 376, 370, 364, 357, 351, 344,
    337, 331, 324,  317, 310, 302, 295, 288, 280,
    273, 265, 257, 249, 242, 234, 226, 218, 209,
    201, 193, 184, 176, 168, 159, 151, 142, 133,
    125, 116, 107, 98, 89, 81, 72, 63, 54, 45, 36,
    27, 18, 9, 0
};

static uint16_t POINT_NUM = sizeof( indexWave )/sizeof( indexWave[0] );
static uint16_t indexWave_pos = 0;

static bool led_step = false;
static uint32_t led_start = 0;

uint8_t app_led_state = APP_LED_IDLE;

static void hal_pwm_handler( nrf_drv_pwm_evt_type_t event_type )
{
    if( event_type == NRF_DRV_PWM_EVT_FINISHED )
    {
        uint16_t * p_channels = ( uint16_t * )( &m_pwm_seq_values );
        p_channels[0] = indexWave[indexWave_pos] * m_pwm_top / 512;
        
        indexWave_pos ++;
        if( indexWave_pos >= POINT_NUM )
        {
            indexWave_pos = 0; 
        }
    }
}

void app_led_breathe_start( void )
{
    app_led_state = APP_LED_LORA_JOINING;
    hal_gpio_set_value( USER_LED_R, 0 );
    if( pwm_init == false )
    {
        pwm_init = true;
        nrf_drv_pwm_config_t const config0 =
        {
            .output_pins =
            {
                USER_LED_G | NRF_DRV_PWM_PIN_INVERTED, // channel 0
                NRF_DRV_PWM_PIN_NOT_USED, // channel 1
                NRF_DRV_PWM_PIN_NOT_USED, // channel 2
                NRF_DRV_PWM_PIN_NOT_USED  // channel 3
            },
            .irq_priority = APP_IRQ_PRIORITY_LOWEST,
            .base_clock   = NRF_PWM_CLK_1MHz,
            .count_mode   = NRF_PWM_MODE_UP_AND_DOWN,
            .top_value    = m_pwm_top,
            .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
            .step_mode    = NRF_PWM_STEP_AUTO
        };
        APP_ERROR_CHECK(nrf_drv_pwm_init( &PWM2, &config0, hal_pwm_handler ));

        m_pwm_seq_values.channel_0 = 0;
        m_pwm_seq_values.channel_1 = 0;
        m_pwm_seq_values.channel_2 = 0;
        m_pwm_seq_values.channel_3 = 0;

        ( void )nrf_drv_pwm_simple_playback( &PWM2, &m_pwm_seq, 1, NRF_DRV_PWM_FLAG_LOOP );
    }
}

void app_led_breathe_stop( void )
{
    app_led_state = APP_LED_IDLE;
    if( pwm_init == true )
    {
        pwm_init = false;
        nrf_drv_pwm_uninit( &PWM2 );
    }
}

void app_user_led_event_timeout_handler( void )
{
    switch( app_led_state )
    {
        case APP_LED_BLE_CFG:
        {
            if( led_step )
            {
                led_step = false;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 1000 ), NULL );
            }
            else
            {
                led_step = true;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
            }
        }
        break;

        case APP_LED_LORA_JOINED:
        {
            if(( hal_rtc_get_time_ms( ) - led_start ) > 2000 )
            {
                app_led_idle( );
                return;
            }

            if( led_step )
            {
                led_step = false;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
            }
            else
            {
                led_step = true;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
            }
        }
        break;

        case APP_LED_SOS:
        {
            if( button_sos_type == 0 ) // SOS single
            {
                if(( hal_rtc_get_time_ms( ) - led_start ) > 3000 )
                {
                    app_led_idle( );
                    return;
                }
            }

            if( led_step )
            {
                led_step = false;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
            }
            else
            {
                led_step = true;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
            }
        }
        break;

        case APP_LED_SOS_CONFIRM:
        {
            led_step = false;
            hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
            app_led_idle( );
        }
        break;

        case APP_LED_LORA_DOENLINK:
        {
            if( led_step )
            {
                led_step = false;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
            }
            else
            {
                led_step = true;
                hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
                app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
            }
        }
        break;

        case APP_LED_IDLE:
        {
            hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
        }
        break;

        default:
        break;
    }
}

void app_led_ble_cfg( void )
{
    led_step = true;
    app_led_state = APP_LED_BLE_CFG;
    hal_gpio_set_value( USER_LED_R, 0 );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
    app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
}

void app_led_lora_joined( void )
{
    led_step = true;
    app_led_state = APP_LED_LORA_JOINED;
    hal_gpio_set_value( USER_LED_R, 0 );
    led_start = hal_rtc_get_time_ms( );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
    app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
}

void app_led_sos_run( void )
{
    led_step = true;
    app_led_state = APP_LED_SOS;
    led_start = hal_rtc_get_time_ms( );
    hal_gpio_set_value( USER_LED_R, 0 );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
    app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
}

void app_led_sos_confirm( void )
{
    led_step = true;
    app_led_state = APP_LED_SOS_CONFIRM;
    hal_gpio_set_value( USER_LED_R, 0 );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
    app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 1000 ), NULL );
}

void app_led_lora_downlink( void )
{
    led_step = true;
    app_led_state = APP_LED_LORA_DOENLINK;
    hal_gpio_set_value( USER_LED_R, 0 );
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_SET );
    app_timer_start( m_led_event_timer_id,  APP_TIMER_TICKS( 500 ), NULL );
}

void app_led_idle( void )
{
    hal_gpio_init_out( USER_LED_G, HAL_GPIO_RESET );
    app_timer_stop( m_led_event_timer_id );
    app_led_state = APP_LED_IDLE;
}

void app_user_bat_event_timeout_handler( void )
{
    app_timer_start( m_bat_event_timer_id,  APP_TIMER_TICKS( 3000 ), NULL );
    if( hal_gpio_get_value( CHARGER_ADC_DET )) // usb detect
    {
        if( hal_gpio_get_value( CHARGER_DONE ) == 0 ) // charge done
        {
            if( app_led_state == APP_LED_IDLE )
            {
                hal_gpio_set_value( USER_LED_R, 0 );
            }
        }
        else if( hal_gpio_get_value( CHARGER_CHRG ) == 0 )
        {
            if( app_led_state == APP_LED_IDLE )
            {
                hal_gpio_set_value( USER_LED_R, 1 ); // charge ongoing
            }
        }
        else
        {
            if( app_led_state == APP_LED_IDLE )
            {
                hal_gpio_toggle( USER_LED_R ); // charge error
            }
        }
    }
    else
    {
        hal_gpio_set_value( USER_LED_R, 0 );
    }
}

void app_led_bat_new_detect( uint32_t time )
{
    app_timer_stop( m_bat_event_timer_id );
    app_timer_start( m_bat_event_timer_id,  APP_TIMER_TICKS( time ), NULL );
}

void app_led_init( void )
{
    app_timer_create( &m_led_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_led_event_timeout_handler );
    app_timer_create( &m_bat_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_bat_event_timeout_handler );
}
