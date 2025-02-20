
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "app_error.h"
#include "smtc_hal_usb_cdc.h"

const nrf_drv_timer_t TIMER_USB = NRF_DRV_TIMER_INSTANCE(3);

static bool usb_timer_init = false;

/**
 * @brief Handler for timer events.
 */
void timer_usb_event_handler( nrf_timer_event_t event_type, void* p_context )
{
    switch( event_type )
    {
        case NRF_TIMER_EVENT_COMPARE0:
        {
            hal_usb_cdc_event_queue_process( );
        }
        break;

        default:
        break;
    }
}

void hal_usb_timer_init( void )
{
    uint32_t time_ms = 1;
    uint32_t time_ticks;
    uint32_t err_code = NRF_SUCCESS;

    if( usb_timer_init == false )
    {
        usb_timer_init = true;
        nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
        err_code = nrf_drv_timer_init( &TIMER_USB, &timer_cfg, timer_usb_event_handler );
        APP_ERROR_CHECK(err_code);

        time_ticks = nrf_drv_timer_ms_to_ticks( &TIMER_USB, time_ms );
        nrf_drv_timer_extended_compare( &TIMER_USB, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true );
        nrf_drv_timer_enable( &TIMER_USB );
    }
}

void hal_usb_timer_uninit( void )
{
    if( usb_timer_init )
    {
        usb_timer_init = false;
        nrf_drv_timer_disable( &TIMER_USB );
        nrf_drv_timer_uninit( &TIMER_USB );
    }
}