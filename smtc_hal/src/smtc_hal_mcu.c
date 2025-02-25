
#include "nrf_nvic.h"
#include "nrf52840.h"
#include "smtc_hal.h"
#include "nrf_pwr_mgmt.h"
#include "smtc_hal_rtc.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "hardfault.h"

static bool m_sleep_enable = false;
static uint32_t m_usb_detect = false;
static bool m_hal_sleep_break = false;

void usb_irq_handler( void *obj );
hal_gpio_irq_t usb_irq = {
    .pin = CHARGER_ADC_DET,
    .callback = usb_irq_handler,
    .context = NULL,
};

void usb_irq_handler( void *obj )
{
    m_usb_detect = hal_gpio_get_value( CHARGER_ADC_DET );
    if( m_usb_detect ) hal_usb_timer_init( );
    else hal_usb_timer_uninit( );
#ifdef APP_TRACKER
    app_led_bat_new_detect( 300 );
#endif
}
void hal_usb_det_init( void )
{
    hal_gpio_init_in( CHARGER_ADC_DET, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_RISING_FALLING, &usb_irq );
    m_usb_detect = hal_gpio_get_value( CHARGER_ADC_DET );
    hal_usb_timer_init( );
}

void hal_clock_init(void)
{
    nrf_drv_clock_init( );
    nrf_drv_clock_lfclk_request( NULL );
}

void hal_pwr_init(void)
{
    nrf_pwr_mgmt_init( );
}

void hal_mcu_init( void )
{
    hal_clock_init( );
    hal_pwr_init( );
    hal_flash_init( );
    hal_gpio_init( );
    hal_rtc_init( );
    hal_spi_init( );
    hal_i2c_init( );
    hal_rng_init( );
    hal_usb_det_init( );
    hal_usb_cdc_init( );
#ifdef APP_TRACKER
    hal_watchdog_init( );
#endif
}

void hal_mcu_disable_irq( void )
{
    __disable_irq();
}

void hal_mcu_enable_irq( void )
{
    __enable_irq();
}

void hal_mcu_reset( void )
{
    sd_nvic_SystemReset( );
}

void __attribute__(( optimize( "O0" ))) hal_mcu_wait_us( const int32_t microseconds )
{
    // Work @64MHz
    const uint32_t nb_nop = microseconds * 1000 / 171;
    for( uint32_t i = 0; i < nb_nop; i++ )
    {
        __NOP( );
    }
}

void hal_mcu_wait_ms( const int32_t ms )
{
    for( uint32_t i = 0; i < ms; i++ )
        hal_mcu_wait_us( 1000 );
}

void hal_mcu_partial_sleep_enable( bool enable )
{
    m_sleep_enable = enable;
}

void hal_mcu_set_sleep_for_ms( const int32_t milliseconds )
{
    bool last_sleep_loop = false;
    int32_t time_counter = milliseconds;

    if( milliseconds <= 0 ) return;

    do
    {
        int32_t time_sleep = 0;
#ifdef APP_TRACKER
        float time_sleep_max = NRFX_WDT_CONFIG_RELOAD_VALUE - 30000; // 60s
#else
        float time_sleep_max = RTC_2_PER_TICK * RTC_2_MAX_TICKS;
#endif
        if( time_counter > time_sleep_max )
        {
            time_sleep = time_sleep_max;
            time_counter -= time_sleep;
        }
        else
        {
            time_sleep = time_counter;
            last_sleep_loop = true;
        }

        if( time_sleep > 50 )
        {
#ifdef APP_TRACKER
            hal_watchdog_reload( );
#endif
            if( m_usb_detect )
            {
                uint32_t current = hal_rtc_get_time_ms( );
                while(( hal_rtc_get_time_ms( ) - current ) < time_sleep )
                {
#ifdef APP_TRACKER
                    if( m_hal_sleep_break )
                    {
                        m_hal_sleep_break = false;
                        break;
                    }

                    app_user_run_process( );
#endif
                    if( m_usb_detect == 0 )
                    {
                        hal_usb_timer_uninit( );
                        break; // usb remove
                    }
                }
            }
            else
            {
#ifdef APP_TRACKER
                app_user_run_process( );
#endif
                hal_rtc_wakeup_timer_set_ms( time_sleep );
                nrf_pwr_mgmt_run( );
            }
        }
    } while( last_sleep_loop == false );
}

void hal_hex_to_bin( char *input, uint8_t *dst, int len )
{
    char tmp[3];
    uint16_t length = strlen( input );
    tmp[2] = NULL;
    for( int i = 0; i < length; i+=2 )
    {
        tmp[0] = input[i];
        tmp[1] = input[i+1];
        dst[i/2] = ( uint8_t )strtol(( const char * )tmp, NULL, 16 );
        if (i >= (2 * len )) break;
    }
}

void hal_print_bin_to_hex( uint8_t *buf, uint16_t len )
{
    for( uint16_t i = 0; i < len; i++ )
    {
        PRINTF( "%02X", buf[i] );
    }
    PRINTF( "\r\n" );
}

void memcpyr( uint8_t *dst, const uint8_t *src, uint16_t size )
{
    dst = dst + ( size - 1 );
    while( size-- )
    {
        *dst-- = *src++;
    }
}

void hal_sleep_exit( void )
{
    m_hal_sleep_break = true;
}

void app_error_fault_handler( uint32_t id, uint32_t pc, uint32_t info )
{
    hal_mcu_wait_ms( 200 );
    NVIC_SystemReset( );
}
