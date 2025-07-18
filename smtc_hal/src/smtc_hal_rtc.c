
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "smtc_hal_rtc.h"
#include "smtc_hal_lp_time.h"
#include "smtc_hal_dbg_trace.h"

#if 0
static const nrf_drv_rtc_t rtc_2 = NRF_DRV_RTC_INSTANCE( 2 );
static nrf_drv_rtc_config_t rtc_config_2 = NRF_DRV_RTC_DEFAULT_CONFIG;
static uint64_t rtc_2_tick_diff = 0;

static void rtc_2_handler( nrf_drv_rtc_int_type_t int_type )
{    
    if( int_type == NRF_DRV_RTC_INT_OVERFLOW )
    {
        rtc_2_tick_diff += RTC_2_MAX_TICKS;
    }
}

void hal_rtc_init( void )
{
    rtc_config_2.prescaler = 2; // base tick is 0.091552734375 ms
    nrf_drv_rtc_init( &rtc_2, &rtc_config_2, rtc_2_handler );
    nrf_drv_rtc_overflow_enable( &rtc_2,true );
    nrf_drv_rtc_enable( &rtc_2 );
}

uint32_t hal_rtc_get_time_s( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK / 1000;
    return temp;
}

uint32_t hal_rtc_get_time_ms( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK;
    return temp;
}

uint32_t hal_rtc_get_time_100us( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK * 10;
    return temp;
}

uint32_t hal_rtc_get_max_ticks( void )
{
	return nrf_drv_rtc_max_ticks_get( &rtc_2 );
}

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds )
{
    uint32_t temp = 0;
    float temp_f = milliseconds;
    temp_f = temp_f / RTC_2_PER_TICK + 1;
    temp = nrf_drv_rtc_counter_get( &rtc_2 ) + temp_f;
    nrf_drv_rtc_cc_set( &rtc_2, 0, temp & RTC_2_MAX_TICKS, true );
}

void hal_rtc_wakeup_timer_stop( void )
{
	
}
#endif

#if 1
static const nrf_drv_rtc_t rtc_2 = NRF_DRV_RTC_INSTANCE( 2 );
static nrf_drv_rtc_config_t rtc_config_2 = NRF_DRV_RTC_DEFAULT_CONFIG;
static uint64_t rtc_2_tick_diff = 0;

static void rtc_2_handler( nrf_drv_rtc_int_type_t int_type )
{
    if( int_type == NRF_DRV_RTC_INT_COMPARE1 )
    {
        hal_lp_timer_event_handler( );
    }
    else if( int_type == NRF_DRV_RTC_INT_OVERFLOW )
    {
        rtc_2_tick_diff += RTC_2_MAX_TICKS;
    }
}

void hal_rtc_init( void )
{
    rtc_config_2.prescaler = 2; // base tick is 0.091552734375 ms
    nrf_drv_rtc_init( &rtc_2, &rtc_config_2, rtc_2_handler );
    nrf_drv_rtc_overflow_enable( &rtc_2,true );
    nrf_drv_rtc_enable( &rtc_2 );
}

uint32_t hal_rtc_get_time_s( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK / 1000;
    return temp;
}

uint32_t hal_rtc_get_time_ms( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK;
    return temp;
}

uint32_t hal_rtc_get_time_100us( void )
{
    uint64_t temp = nrf_drv_rtc_counter_get( &rtc_2 ) + rtc_2_tick_diff;
    temp = temp * RTC_2_PER_TICK * 10;
    return temp;
}

uint32_t hal_rtc_get_max_ticks( void )
{
    return nrf_drv_rtc_max_ticks_get( &rtc_2 );
}

void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds )
{
    uint32_t temp = 0;
    float temp_f = milliseconds;
    temp_f = temp_f / RTC_2_PER_TICK + 1;
    temp = nrf_drv_rtc_counter_get( &rtc_2 ) + temp_f;
    nrf_drv_rtc_cc_set( &rtc_2, 0, temp & RTC_2_MAX_TICKS, true ); // enable compare counter 0
}

void hal_rtc_wakeup_timer_stop( void )
{
	nrf_drv_rtc_cc_disable( &rtc_2, 0 );  // disable compare counter 0
}

void hal_rtc_cc1_timer_set_ms( const int32_t milliseconds )
{
    uint32_t temp = 0;
    float temp_f = milliseconds;
    temp_f = temp_f / RTC_2_PER_TICK + 1;
    temp = nrf_drv_rtc_counter_get( &rtc_2 ) + temp_f;
    nrf_drv_rtc_cc_set( &rtc_2, 1, temp & RTC_2_MAX_TICKS, true ); // enable compare counter 1
}

void hal_rtc_cc1_timer_stop( void )
{
    nrf_drv_rtc_cc_disable( &rtc_2, 1 );  // disable compare counter 1
}

#endif
