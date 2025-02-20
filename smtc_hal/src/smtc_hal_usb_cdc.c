
#include "app_error.h"
#include "app_usbd_core.h"
#include "app_usbd.h"
#include "app_usbd_string_desc.h"
#include "app_usbd_cdc_acm.h"
#include "app_usbd_serial_num.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "smtc_hal_usb_cdc.h"

static void cdc_acm_user_ev_handler( app_usbd_class_inst_t const * p_inst,  app_usbd_cdc_acm_user_event_t event );

#define CDC_ACM_COMM_INTERFACE  0
#define CDC_ACM_COMM_EPIN       NRF_DRV_USBD_EPIN2

#define CDC_ACM_DATA_INTERFACE  1
#define CDC_ACM_DATA_EPIN       NRF_DRV_USBD_EPIN1
#define CDC_ACM_DATA_EPOUT      NRF_DRV_USBD_EPOUT1

/**
 * @brief CDC_ACM class instance
 * */
APP_USBD_CDC_ACM_GLOBAL_DEF(m_app_cdc_acm,
                            cdc_acm_user_ev_handler,
                            CDC_ACM_COMM_INTERFACE,
                            CDC_ACM_DATA_INTERFACE,
                            CDC_ACM_COMM_EPIN,
                            CDC_ACM_DATA_EPIN,
                            CDC_ACM_DATA_EPOUT,
                            APP_USBD_CDC_COMM_PROTOCOL_AT_V250
);

#define READ_SIZE 1
static char m_rx_buffer[READ_SIZE];
static bool m_send_flag = 0;

static bool m_usb_init = false;
static bool m_usb_connected = false;

#ifdef APP_TRACKER
uint16_t g_usb_rec_index = 0;
uint8_t g_usb_rec_buffer[256] = { 0 };
#endif

/**
 * @brief User event handler @ref app_usbd_cdc_acm_user_ev_handler_t (headphones)
 * */
static void cdc_acm_user_ev_handler( app_usbd_class_inst_t const * p_inst, app_usbd_cdc_acm_user_event_t event )
{
    app_usbd_cdc_acm_t const * p_cdc_acm = app_usbd_cdc_acm_class_get( p_inst );

    switch( event )
    {
        case APP_USBD_CDC_ACM_USER_EVT_PORT_OPEN:
        {
            /*Setup first transfer*/
            ret_code_t ret = app_usbd_cdc_acm_read( &m_app_cdc_acm, m_rx_buffer, READ_SIZE );
            UNUSED_VARIABLE(ret);
            break;
        }

        case APP_USBD_CDC_ACM_USER_EVT_PORT_CLOSE:
            break;

        case APP_USBD_CDC_ACM_USER_EVT_TX_DONE:
            break;
        
        case APP_USBD_CDC_ACM_USER_EVT_RX_DONE:
        {
            ret_code_t ret;
            NRF_LOG_INFO( "Bytes waiting: %d", app_usbd_cdc_acm_bytes_stored( p_cdc_acm ));
            do
            {
                /*Get amount of data transfered*/
                size_t size = app_usbd_cdc_acm_rx_size( p_cdc_acm );
                NRF_LOG_INFO("RX: size: %lu char: %c", size, m_rx_buffer[0] );

#ifdef APP_TRACKER
                g_usb_rec_buffer[g_usb_rec_index ++] = m_rx_buffer[0];
                if( g_usb_rec_index >= sizeof( g_usb_rec_buffer )) g_usb_rec_index = 0;
#endif

                /* Fetch data until internal buffer is empty */
                ret = app_usbd_cdc_acm_read( &m_app_cdc_acm, m_rx_buffer, READ_SIZE );
            } while( ret == NRF_SUCCESS );
            
#ifdef APP_TRACKER
            if(( g_usb_rec_index > 2 ) &&( g_usb_rec_buffer[g_usb_rec_index - 2] == '\r' ) && ( g_usb_rec_buffer[g_usb_rec_index - 1] == '\n' ))
            {
                extern bool button_power;
                if( button_power == false ) return;
                app_user_timer_parse_cmd( 0 );
            }
#endif
            break;
        }
        default:
            break;
    }
}

static void usbd_user_ev_handler( app_usbd_event_type_t event )
{
    switch( event )
    {
        case APP_USBD_EVT_DRV_SUSPEND:
            break;
        case APP_USBD_EVT_DRV_RESUME:
            break;
        case APP_USBD_EVT_STARTED:
            break;
        case APP_USBD_EVT_STOPPED:
            app_usbd_disable( );
            break;
        case APP_USBD_EVT_POWER_DETECTED:
            NRF_LOG_INFO( "USB power detected" );
            if( !nrf_drv_usbd_is_enabled( ))
            {
                app_usbd_enable( );
            }
            break;
        case APP_USBD_EVT_POWER_REMOVED:
            NRF_LOG_INFO( "USB power removed" );
            app_usbd_stop( );
            m_usb_connected = false;
            break;
        case APP_USBD_EVT_POWER_READY:
            NRF_LOG_INFO( "USB ready" );
            app_usbd_start( );
            m_usb_connected = true;
            break;
        default:
            break;
    }
}

void hal_usb_cdc_init( void )
{
    ret_code_t ret;
    static const app_usbd_config_t usbd_config = {
        .ev_state_proc = usbd_user_ev_handler
    };

    while( !nrf_drv_clock_lfclk_is_running( ))
    {
        /* Just waiting */
    }

    app_usbd_serial_num_generate( );

    ret = app_usbd_init( &usbd_config );
    APP_ERROR_CHECK( ret );

    app_usbd_class_inst_t const *class_cdc_acm = app_usbd_cdc_acm_class_inst_get( &m_app_cdc_acm );
    ret = app_usbd_class_append( class_cdc_acm );
    APP_ERROR_CHECK( ret );

    ret = app_usbd_power_events_enable( );
    APP_ERROR_CHECK( ret );

    m_usb_init = true;
}

void hal_usb_cdc_deinit( void )
{
    app_usbd_uninit( );
}

void hal_usb_cdc_write( uint8_t* buff, uint16_t len )
{
    if( m_usb_connected )
    {
        app_usbd_cdc_acm_write( &m_app_cdc_acm, buff, len );
        hal_mcu_wait_us( 1000 );
    }
}

void hal_usb_cdc_read( uint8_t* buff, uint16_t len )
{
    if( m_usb_connected )
    {
        app_usbd_cdc_acm_read( &m_app_cdc_acm, buff, len );
    }
}

void hal_usb_cdc_event_queue_process( void )
{
    if( m_usb_init )
    {
        app_usbd_event_queue_process( );
    }
}

bool hal_usb_cdc_is_connected( void )
{
    return m_usb_connected;
}

