

#include "smtc_hal_dbg_trace.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "smtc_hal_rtc.h"
#include "smtc_hal_lp_time.h"

#if 0
static const nrf_drv_timer_t TIMER_LP = NRF_DRV_TIMER_INSTANCE( 4 );
static hal_lp_timer_irq_t lptim_tmr_irq = { .context = NULL, .callback = NULL };

static bool lp_timer_init = false;

static void timer_event_handler( nrf_timer_event_t event_type, void* p_context)
{
    hal_lp_timer_stop( );
    hal_lp_timer_deinit( );

    if( lptim_tmr_irq.callback != NULL )
    {
        lptim_tmr_irq.callback( lptim_tmr_irq.context );
    }
}

void hal_lp_timer_init( void )
{
    if( lp_timer_init == false )
    {
        lp_timer_init = true;
        nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
        nrf_drv_timer_init( &TIMER_LP, &timer_cfg, timer_event_handler );
    }
}

void hal_lp_timer_deinit( void )
{
    if( lp_timer_init == true )
    {
        lp_timer_init = false;
        nrf_drv_timer_uninit( &TIMER_LP );
    }
}

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq )
{
    uint32_t time_ticks;
    hal_lp_timer_init( );
    time_ticks = nrf_drv_timer_ms_to_ticks( &TIMER_LP, milliseconds );
    nrf_drv_timer_extended_compare( &TIMER_LP, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true );
    lptim_tmr_irq = *tmr_irq;
    nrf_drv_timer_enable( &TIMER_LP );
}

void hal_lp_timer_stop( void )
{
    if( lp_timer_init == true )
    {
        nrf_drv_timer_disable( &TIMER_LP );
    }
}

void hal_lp_timer_irq_enable( void )
{
    // if( lp_timer_init == true )
    // {
    //     nrfx_timer_compare_int_enable( &TIMER_LP, NRF_TIMER_CC_CHANNEL0 );
    // }
}

void hal_lp_timer_irq_disable( void )
{
    // if( lp_timer_init == true )
    // {
    //     nrfx_timer_compare_int_disable( &TIMER_LP, NRF_TIMER_CC_CHANNEL0 );
    // }
}
#endif

#if 1
static bool lp_timer_init = false;
static hal_lp_timer_irq_t lptim_tmr_irq = { .context = NULL, .callback = NULL };

void hal_lp_timer_event_handler( void )
{
    hal_lp_timer_stop( );
    hal_lp_timer_deinit( );

    if( lptim_tmr_irq.callback != NULL )
    {
        lptim_tmr_irq.callback( lptim_tmr_irq.context );
    }
}

void hal_lp_timer_init( void )
{
    if( lp_timer_init == false )
    {
        lp_timer_init = true;
    }
}

void hal_lp_timer_deinit( void )
{
    if( lp_timer_init == true )
    {
        lp_timer_init = false;
    }
}

void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq )
{
    lptim_tmr_irq = *tmr_irq;
    hal_rtc_cc1_timer_set_ms( milliseconds );
}

void hal_lp_timer_stop( void )
{
    if( lp_timer_init == true )
    {
        hal_rtc_cc1_timer_stop( );
    }
}

void hal_lp_timer_irq_enable( void )
{
    
}

void hal_lp_timer_irq_disable( void )
{
    
}
#endif
