
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_timer.h"
#include "app_error.h"
#include "smtc_hal_usb_cdc.h"
#include "app_ble_all.h"
#include "app_at.h"
#include "app_button.h"

APP_TIMER_DEF(m_parse_cmd_timer_id);

static bool parse_cmd_flag = false;

static void app_user_parse_cmd_handler( void )
{
    parse_cmd_flag = true;
}

void app_user_timers_init( void )
{
    ret_code_t err_code;
    err_code = app_timer_create( &m_parse_cmd_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_parse_cmd_handler );
    APP_ERROR_CHECK( err_code );
}

void app_user_timer_parse_cmd( uint8_t cmd_type )
{
    ret_code_t err_code;
    parse_cmd_type = cmd_type;
    err_code = app_timer_start( m_parse_cmd_timer_id,  APP_TIMER_TICKS( 100 ), NULL );
    APP_ERROR_CHECK( err_code );
}

void app_user_parse_cmd( void )
{
    if( parse_cmd_flag )
    {
        parse_cmd_flag = false;
        if( parse_cmd_type )
        {
            parse_cmd( ble_rec_buff, ble_rec_length );
            memset( ble_rec_buff, 0, sizeof( ble_rec_buff ));
            ble_rec_length = 0;
        }
        else
        {
            parse_cmd( g_usb_rec_buffer, g_usb_rec_index );
            memset( g_usb_rec_buffer, 0, sizeof( g_usb_rec_buffer ));
            g_usb_rec_index = 0;
        }
        parse_cmd_type = 0;
    }
}

void app_user_run_process( void )
{
    app_user_parse_cmd( );
    app_user_button_det( );
}