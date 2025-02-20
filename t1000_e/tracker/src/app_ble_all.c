
#include "nordic_common.h"
#include "app_error.h"
#include "nrf_sdm.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_ble_scan.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "smtc_hal.h"
#include "app_config_param.h"
#include "app_ble_nus.h"
#include "app_ble_all.h"

#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO           3                                           /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_SOC_OBSERVER_PRIO           1                                           /**< Applications' SoC observer priority. You shouldn't need to modify this value. */

#define APP_ADV_FAST_INTERVAL           160                                         /**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_FAST_DURATION           0                                           /**< The advertising duration (10 seconds) in units of 10 milliseconds. */
#define APP_ADV_SLOW_INTERVAL           96000       
#define APP_ADV_SLOW_DURATION           0

#define MIN_CONN_INTERVAL               MSEC_TO_UNITS(100, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL               MSEC_TO_UNITS(200, UNIT_1_25_MS)            /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                   0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                       /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)                      /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */

#define DEAD_BEEF                       0xDEADBEEF                                  /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define APP_SCAN_INTERVAL               160                                         /**< Determines scan interval(in units of 0.625 ms). */
#define APP_SCAN_WINDOW                 80                                          /**< Determines scan window(in units of 0.625 ms). */
#define APP_SCAN_DURATION               0                                           /**< Duration of the scanning(in units of 10 ms). */

NRF_BLE_SCAN_DEF(m_scan);               /**< Scanning Module instance. */

/**< Scan parameters requested for scanning and connection. */
static ble_gap_scan_params_t const m_scan_param =
{
    .active        = 0x00,
    .interval      = APP_SCAN_INTERVAL,
    .window        = APP_SCAN_WINDOW,
    .timeout       = APP_SCAN_DURATION,
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
    .scan_phys     = BLE_GAP_PHY_1MBPS,
};

#define BEACON_DATA_LEN     0x15
#define BEACON_DATA_TYPE    0x02
#define COMPANY_IDENTIFIER  0x004C

#define BLE_BEACON_BUF_MAX  16
#define BLE_BEACON_SEND_MUM 5

bool s_filter_flag = false;
uint8_t ble_beacon_res_num = 0;
BleBeacons_t ble_beacon_buf[BLE_BEACON_BUF_MAX] = { 0 };
uint8_t ble_beacon_rssi_array[BLE_BEACON_BUF_MAX] = { 0 };
uint8_t ble_uuid_filter_array[16] = { 0 };
uint8_t ble_uuid_filter_num = 0;

BLE_NUS_DEF(m_nus, NRF_SDH_BLE_TOTAL_LINK_COUNT);

NRF_BLE_GATT_DEF(m_gatt);                                                           /**< GATT module instance. */
NRF_BLE_QWR_DEF(m_qwr);                                                             /**< Context for the Queued Write module.*/
BLE_ADVERTISING_DEF(m_advertising);                                                 /**< Advertising module instance. */

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;                            /**< Handle of the current connection. */

uint16_t ble_rec_length = 0;
uint8_t  ble_rec_buff[64];
uint8_t  is_notify_enable = 0;
bool is_Communicate_with_app = false;
static char ble_tx_buf[HAL_PRINT_BUFFER_SIZE];

static uint8_t app_ble_state = BLE_GAP_EVT_DISCONNECTED;
static uint16_t m_ble_nus_max_data_len = BLE_GATT_ATT_MTU_DEFAULT - 3; 

static ble_uuid_t m_adv_uuids[] = 
{
    { 0x2886, BLE_UUID_TYPE_BLE },
    { 0xA886, BLE_UUID_TYPE_BLE }
};

static void advertising_start( void * p_erase_bonds );
static void send_data_to_ble( uint8_t* buffer,uint16_t length );
static bool buf_cmp_value( uint8_t *a, uint8_t *b, uint8_t len );
static bool beacon_uuid_filter( uint8_t *uuid );


/**@brief Function for assert macro callback.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyse
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num    Line number of the failing ASSERT call.
 * @param[in] p_file_name File name of the failing ASSERT call.
 */
void assert_nrf_callback( uint16_t line_num, const uint8_t * p_file_name )
{
    app_error_handler( DEAD_BEEF, line_num, p_file_name );
}

/**@brief Function for initializing the timer module.
 */
static void timers_init( void )
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

static void disconnect(uint16_t conn_handle, void * p_context )
{
    UNUSED_PARAMETER( p_context );

    ret_code_t err_code = sd_ble_gap_disconnect( conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION );
    if( err_code != NRF_SUCCESS )
    {
        NRF_LOG_WARNING( "Failed to disconnect connection. Connection handle: %d Error: %d", conn_handle, err_code );
    }
    else
    {
        NRF_LOG_DEBUG( "Disconnected connection handle %d", conn_handle );
    }
}

static void advertising_config_get( ble_adv_modes_config_t * p_config )
{
    memset( p_config, 0, sizeof( ble_adv_modes_config_t ));

    p_config->ble_adv_fast_enabled  = true;
    p_config->ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    p_config->ble_adv_fast_timeout  = APP_ADV_FAST_DURATION;
    p_config->ble_adv_slow_enabled  = false;
    p_config->ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
    p_config->ble_adv_slow_timeout  = APP_ADV_SLOW_DURATION;
}

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init( void )
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
    uint8_t adv_device_name[24] = { 0 };
    uint32_t ble_mac_lsb = NRF_FICR->DEVICEADDR[0];
    sprintf( adv_device_name, "T1000-E %02X%02X", ( ble_mac_lsb >> 8 )& 0xff, ble_mac_lsb & 0xff );

    BLE_GAP_CONN_SEC_MODE_SET_OPEN( &sec_mode );
    err_code = sd_ble_gap_device_name_set( &sec_mode, (const uint8_t *)adv_device_name, strlen( adv_device_name ));
    APP_ERROR_CHECK( err_code );

    /* YOUR_JOB: Use an appearance value matching the application's use case.
       err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_);
       APP_ERROR_CHECK(err_code); */

    memset( &gap_conn_params, 0, sizeof( gap_conn_params ));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set( &gap_conn_params );
    APP_ERROR_CHECK( err_code );
}

/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void nrf_qwr_error_handler( uint32_t nrf_error )
{
    APP_ERROR_HANDLER( nrf_error );
}

/* add nus service handler function */
static void nus_data_handler( ble_nus_evt_t * p_evt )
{
    uint32_t err_code;
    switch( p_evt->type )
    {
        case BLE_NUS_EVT_RX_DATA: // 
        {
            NRF_LOG_DEBUG( "Received data from BLE NUS. Writing data on UART." );
            NRF_LOG_HEXDUMP_DEBUG( p_evt->params.rx_data.p_data, p_evt->params.rx_data.length );
            // PRINTF( "nus_data_handler:%s\r\n", p_evt->params.rx_data.p_data ); //< Data received. 
            memcpy( ble_rec_buff, p_evt->params.rx_data.p_data, p_evt->params.rx_data.length );
            ble_rec_length = p_evt->params.rx_data.length;
            app_user_timer_parse_cmd( 1 );
        }

        break;
        case BLE_NUS_EVT_TX_RDY: // Service is ready to accept new data to be transmitted
        {

        }
        break;
        case BLE_NUS_EVT_COMM_STARTED: // Notification has been enabled
        {
            is_notify_enable = 1;
            is_Communicate_with_app = true;
            // TODO

        }
        
        break;  
        case BLE_NUS_EVT_COMM_STOPPED: // Notification has been disabled
        {
            is_notify_enable = 0;
        }
        break;

        default:
        break;              
    
    }  
}

static void send_data_to_ble( uint8_t* buffer,uint16_t length )
{
    uint32_t err_code;
    do 
    {
        err_code = ble_nus_data_send( &m_nus, buffer, &length, m_conn_handle );
        if(( err_code != NRF_ERROR_INVALID_STATE ) && ( err_code != NRF_ERROR_RESOURCES) && ( err_code != NRF_ERROR_NOT_FOUND ))
        {
            APP_ERROR_CHECK( err_code );
        }
    } while( err_code == NRF_ERROR_RESOURCES );
}

static void vprint( const char* fmt, va_list argp )
{
    if( 0 < vsprintf( ble_tx_buf, fmt, argp ) )  // build string
    {
        send_data_to_ble( ble_tx_buf, strlen( ble_tx_buf ));
    }
}

static void services_init( void )
{
    uint32_t                  err_code;
    nrf_ble_qwr_init_t        qwr_init  = { 0 };
    ble_nus_init_t            nus_init  = { 0 };


    // Initialize Queued Write Module.
    qwr_init.error_handler = nrf_qwr_error_handler;
    err_code = nrf_ble_qwr_init( &m_qwr, &qwr_init );
    APP_ERROR_CHECK( err_code );

    nus_init.data_handler = nus_data_handler;
    err_code = ble_nus_init( &m_nus, &nus_init );
    APP_ERROR_CHECK( err_code );
}

/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt( ble_conn_params_evt_t * p_evt )
{
    uint32_t err_code;

    if( p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED )
    {
        err_code = sd_ble_gap_disconnect( m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE );
        APP_ERROR_CHECK( err_code );
    }
}

/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler( uint32_t nrf_error )
{
    APP_ERROR_HANDLER( nrf_error );
}

/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init( void )
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset( &cp_init, 0, sizeof( cp_init ));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init( &cp_init );
    APP_ERROR_CHECK( err_code );
}

/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter( void )
{   
    // PRINTF( "ble adv idle\r\n" );
}

/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt( ble_adv_evt_t ble_adv_evt )
{
    uint32_t err_code;

    switch( ble_adv_evt )
    {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO( "BLE_ADV_EVT_FAST\r\n" );
            // PRINTF( "BLE_ADV_EVT_FAST\r\n" );
        break;

        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter( );
        break;

        default:
        break;
    }
}
/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler( ble_evt_t const * p_ble_evt, void * p_context )
{
    ret_code_t            err_code;
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch( p_ble_evt -> header.evt_id )
    {
        case BLE_GAP_EVT_ADV_REPORT:
        {
            if( m_scan.scan_buffer.len )
            {
                uint8_t *p_data = &m_scan.scan_buffer.p_data[5];
                uint8_t beacon_data_len = 0;
                uint8_t beacon_data_type = 0;
                uint16_t company_identifier = 0;
                beacon_data_len = p_data[3];
                beacon_data_type = p_data[2];
                memcpy(( uint8_t * )( &company_identifier ), p_data, 2 );
                if( beacon_data_type == BEACON_DATA_TYPE )
                {
                    if( company_identifier == COMPANY_IDENTIFIER )
                    {
                        if( beacon_data_len == BEACON_DATA_LEN )
                        {
                            s_filter_flag = true;
                            // HAL_DBG_TRACE_PRINTF( "iBeacon: " );
                            // for( uint8_t i = 0; i < beacon_data_len; i++ )
                            // HAL_DBG_TRACE_PRINTF( "%02x ", p_data[i] );
                            // HAL_DBG_TRACE_PRINTF( "\r\n" );

                            if( beacon_uuid_filter(( uint8_t * )p_data + 4 ) == false ) return;

                            bool res0 = true, res1 = true, res2 = true, res3 = true, res4 = true, res5 = true;
                            for( uint8_t j = 0; j < BLE_BEACON_BUF_MAX; j++ )
                            {
                                res1 = buf_cmp_value( ble_beacon_buf[j].uuid, ( uint8_t *)( p_data + 4 ), 16 );
                                res2 = buf_cmp_value(( uint8_t *)( &ble_beacon_buf[j].major ), ( uint8_t *)( p_data + 20 ), 2 );
                                res3 = buf_cmp_value(( uint8_t *)( &ble_beacon_buf[j].minor ), ( uint8_t *)( p_data + 22 ), 2 );
                                // res4 = buf_cmp_value(( uint8_t *)( &ble_beacon_buf[j].rssi ), ( uint8_t *)( p_gap_evt->params.adv_report.rssi + 24 ), 1 );
                                res5 = buf_cmp_value(( uint8_t *)( &ble_beacon_buf[j].mac ), ( uint8_t *)( p_gap_evt->params.adv_report.peer_addr.addr ), 6 );
                                if( res1 && res2 && res3 && res4 && res5 ) // all is same, don't save the scan result
                                {
                                    res0 = false;
                                    break;
                                }
                            }
                            if( res0 )
                            {
                                if(( ble_beacon_res_num < BLE_BEACON_BUF_MAX ) && ( ble_beacon_buf[ble_beacon_res_num].company_id == 0 ))
                                {
                                    ble_beacon_buf[ble_beacon_res_num].company_id = company_identifier;
                                    memcpy( ble_beacon_buf[ble_beacon_res_num].uuid, p_data + 4, 16 );
                                    memcpy(( uint8_t *)( &ble_beacon_buf[ble_beacon_res_num].major ), p_data + 20, 2 );
                                    memcpy(( uint8_t *)( &ble_beacon_buf[ble_beacon_res_num].minor ), p_data + 22, 2 );
                                    memcpy(( uint8_t *)( &ble_beacon_buf[ble_beacon_res_num].rssi ), p_data + 24, 1 );
                                    memcpy(( uint8_t *)( &ble_beacon_buf[ble_beacon_res_num].rssi_ ), ( uint8_t *)( &p_gap_evt->params.adv_report.rssi ), 1 );
                                    memcpy(( uint8_t *)( &ble_beacon_buf[ble_beacon_res_num].mac ), ( uint8_t *)( p_gap_evt->params.adv_report.peer_addr.addr ), 6 );
                                    ble_beacon_res_num ++;
                                    break;
                                }   
                            }
                        }
                    }
                }
            }
        } break;

        case BLE_GAP_EVT_DISCONNECTED:
            app_ble_state = BLE_GAP_EVT_DISCONNECTED;
            NRF_LOG_DEBUG( "BLE_GAP_EVT_DISCONNECTED\r\n" );
            // PRINTF( "BLE_GAP_EVT_DISCONNECTED\r\n" );
            sd_ble_gap_adv_stop( m_advertising.adv_handle );
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            
            // reboot device
            hal_mcu_reset( );
        break;

        case BLE_GAP_EVT_CONNECTED:
            app_ble_state = BLE_GAP_EVT_CONNECTED;
            NRF_LOG_DEBUG( "BLE_GAP_EVT_CONNECTED\r\n" );
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign( &m_qwr, m_conn_handle );
            APP_ERROR_CHECK( err_code );
        break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set( m_conn_handle, NULL, 0, 0 );
            APP_ERROR_CHECK( err_code );
        break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            NRF_LOG_DEBUG( "PHY update request." );
            ble_gap_phys_t const phys =
            {
                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update( p_ble_evt->evt.gap_evt.conn_handle, &phys );
            APP_ERROR_CHECK( err_code );
            break;
        }

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG( "GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect( p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION );
            APP_ERROR_CHECK( err_code );
        break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG( "GATT Server Timeout." );
            err_code = sd_ble_gap_disconnect( p_ble_evt->evt.gatts_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION );
            APP_ERROR_CHECK( err_code );
        break;

        default:
        break;
    }
}

/**
 * @brief SoftDevice SoC event handler.
 *
 * @param[in] evt_id    SoC event.
 * @param[in] p_context Context.
 */
static void soc_evt_handler( uint32_t evt_id, void * p_context )
{
    switch( evt_id )
    {
        default:
            break;
    }
}

static void scan_evt_handler( scan_evt_t const * p_scan_evt )
{
    ret_code_t err_code;
    switch( p_scan_evt -> scan_evt_id) 
    {
        case NRF_BLE_SCAN_EVT_SCAN_TIMEOUT:
        {
            HAL_DBG_TRACE_ERROR( "Scan timed out\r\n" );
        } break;

        default:
          break;
    }
}

static void ble_stack_init( void )
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request( );
    APP_ERROR_CHECK( err_code );

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set( APP_BLE_CONN_CFG_TAG, &ram_start );
    APP_ERROR_CHECK( err_code );

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable( &ram_start );
    APP_ERROR_CHECK( err_code );

    // Register handlers for BLE and SoC events.
    NRF_SDH_BLE_OBSERVER( m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL );
    NRF_SDH_SOC_OBSERVER( m_soc_observer, APP_SOC_OBSERVER_PRIO, soc_evt_handler, NULL );
}

static void scan_init( void )
{
    ret_code_t err_code;
    nrf_ble_scan_init_t init_scan;

    memset( &init_scan, 0, sizeof( init_scan ));

    init_scan.connect_if_match = false;
    init_scan.conn_cfg_tag     = APP_BLE_CONN_CFG_TAG;
    init_scan.p_scan_param     = &m_scan_param;

    err_code = nrf_ble_scan_init( &m_scan, &init_scan, scan_evt_handler );
    APP_ERROR_CHECK( err_code );
}

static bool buf_cmp_value( uint8_t *a, uint8_t *b, uint8_t len )
{
    for( uint8_t i = 0; i < len; i++ )
    {
        if( a[i] != b[i] )
        {
            return false;
        }
    }
    return true;
}

static bool beacon_uuid_filter( uint8_t *uuid )
{
    char filter_str[36] = { 0 }, raw_str[36] = { 0 };
    
    if( ble_uuid_filter_num == 0 ) return true; // not use uuid filter
    
    for( uint8_t i = 0; i < 16; i++ )
    {
        sprintf( raw_str + i * 2, "%02X", uuid[i] );
    }
    for( uint8_t i = 0; i < ble_uuid_filter_num; i++ )
    {
        sprintf( filter_str + i * 2, "%02X", ble_uuid_filter_array[i] );
    }

    if( strstr( raw_str, filter_str )) return true;
    else return false;
}

bool ble_scan_start( void )
{
    ret_code_t err_code;

    for( uint8_t i = 0; i < BLE_BEACON_BUF_MAX; i++ )
    {
        memset(( uint8_t *)( &ble_beacon_buf[i] ), 0, sizeof( BleBeacons_t ));    
    }
    ble_beacon_res_num = 0;

    err_code = nrf_ble_scan_params_set( &m_scan, &m_scan_param );
    APP_ERROR_CHECK( err_code );

    err_code = nrf_ble_scan_start( &m_scan );
    APP_ERROR_CHECK( err_code );
}

bool ble_get_results( uint8_t *result, uint8_t *size )
{
    uint8_t beacon_num = 0;
    int8_t rssi_max_index = 0;

    if( result && size )
    {
        *size = 0;
        if( ble_beacon_res_num == 0 ) return false;
        
        for( uint8_t i = 0; i < ble_beacon_res_num; i++ )
        {
            ble_beacon_rssi_array[i] = i;
        }

        for( uint8_t i = 0; i < ble_beacon_res_num; i++ )
        {
            for( uint8_t j = i; j < ble_beacon_res_num; j++ )
            {
                if( ble_beacon_buf[ble_beacon_rssi_array[i]].rssi_ < ble_beacon_buf[ble_beacon_rssi_array[j]].rssi_ )
                {
                    rssi_max_index = ble_beacon_rssi_array[i];
                    ble_beacon_rssi_array[i] = ble_beacon_rssi_array[j];
                    ble_beacon_rssi_array[j] = rssi_max_index;
                }
            }
        }

        if( ble_beacon_res_num > BLE_BEACON_SEND_MUM )
        {
            beacon_num = BLE_BEACON_SEND_MUM;
        }
        else
        {
            beacon_num = ble_beacon_res_num;
        }

        for( uint8_t i = 0; i < beacon_num; i ++ )
        {
            memcpyr( result + i * 7, ( uint8_t *)( &ble_beacon_buf[ble_beacon_rssi_array[i]].mac ), 6 );
            memcpy( result + i * 7 + 6, &ble_beacon_buf[ble_beacon_rssi_array[i]].rssi_, 1 );
            *size += 7;
        }

        // if( beacon_num < BLE_BEACON_SEND_MUM )
        // {
        //     for( uint8_t i = 0; i < ( BLE_BEACON_SEND_MUM - beacon_num ); i++ )
        //     {
        //         memset( result + beacon_num * 7 + i * 7, 0xff, 7 );
        //         *size += 7;
        //     }
        // }
        
        if( beacon_num ) return true;
        else return false;
    }
    return false;
}

void ble_scan_stop( void )
{
    nrf_ble_scan_stop( );
}

void ble_display_results( void )
{
    HAL_DBG_TRACE_PRINTF( "iBeacon: %d\r\n", ble_beacon_res_num );
    for( uint8_t i = 0; i < ble_beacon_res_num; i ++ )
    {
        HAL_DBG_TRACE_PRINTF("%04x, ", ble_beacon_buf[ble_beacon_rssi_array[i]].company_id );
        for( uint8_t j = 0; j < 16; j++ )
        {
            HAL_DBG_TRACE_PRINTF( "%02x ", ble_beacon_buf[ble_beacon_rssi_array[i]].uuid[j] );
        }

        uint16_t major = 0, minor = 0;
        memcpyr(( uint8_t *)( &major ), ( uint8_t *)( &ble_beacon_buf[ble_beacon_rssi_array[i]].major ), 2 );
        memcpyr(( uint8_t *)( &minor ), ( uint8_t *)( &ble_beacon_buf[ble_beacon_rssi_array[i]].minor ), 2 );
        HAL_DBG_TRACE_PRINTF(", %d, \t", major );
        HAL_DBG_TRACE_PRINTF("%d, \t", minor );
        HAL_DBG_TRACE_PRINTF("%d/%d dBm, ", ble_beacon_buf[ble_beacon_rssi_array[i]].rssi, ble_beacon_buf[ble_beacon_rssi_array[i]].rssi_ );

        for( uint8_t j = 0; j < 5; j++ )
        {
            HAL_DBG_TRACE_PRINTF( "%02x:", ble_beacon_buf[ble_beacon_rssi_array[i]].mac[5 - j] );
        }
        HAL_DBG_TRACE_PRINTF( "%02x\r\n", ble_beacon_buf[ble_beacon_rssi_array[i]].mac[0] );
    }
    HAL_DBG_TRACE_PRINTF( "\n" );
}

static void advertising_init( void )
{
    uint32_t               err_code;
    ble_advertising_init_t init;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance      = false;
    init.advdata.uuids_complete.uuid_cnt = sizeof( m_adv_uuids ) / sizeof( m_adv_uuids[0] );
    init.advdata.uuids_complete.p_uuids  = m_adv_uuids;

    advertising_config_get( &init.config );

    init.evt_handler = on_adv_evt;

    err_code = ble_advertising_init( &m_advertising, &init );
    APP_ERROR_CHECK( err_code );

    ble_advertising_conn_cfg_tag_set( &m_advertising, APP_BLE_CONN_CFG_TAG );
}

/**@brief Function for handling events from the GATT library. */
void gatt_evt_handler( nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * p_evt )
{
    if(( m_conn_handle == p_evt->conn_handle ) && ( p_evt->evt_id == NRF_BLE_GATT_EVT_ATT_MTU_UPDATED ))
    {
        m_ble_nus_max_data_len = p_evt->params.att_mtu_effective - OPCODE_LENGTH - HANDLE_LENGTH;
        NRF_LOG_INFO( "Data len is set to 0x%X(%d)", m_ble_nus_max_data_len, m_ble_nus_max_data_len );
    }
    NRF_LOG_DEBUG( "ATT MTU exchange completed. central 0x%x peripheral 0x%x", p_gatt->att_mtu_desired_central, p_gatt->att_mtu_desired_periph );
}

/**@brief   Function for initializing the GATT module.
 * @details The GATT module handles ATT_MTU and Data Length update procedures automatically.
 */
static void gatt_init( void )
{
    ret_code_t err_code = nrf_ble_gatt_init( &m_gatt, NULL );
    // ret_code_t err_code = nrf_ble_gatt_init( &m_gatt, gatt_evt_handler );
    APP_ERROR_CHECK( err_code );
    err_code = nrf_ble_gatt_att_mtu_periph_set( &m_gatt, NRF_SDH_BLE_GATT_MAX_MTU_SIZE );
    APP_ERROR_CHECK( err_code );
}

/**@brief Function for starting advertising.
 */
void advertising_start( void * p_erase_bonds )
{
    uint32_t err_code = ble_advertising_start( &m_advertising, BLE_ADV_MODE_FAST );
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEBUG( "advertising is started" );
}

void app_ble_all_init( void )
{
    ret_code_t err_code;

    // timers_init( );
    ble_stack_init( ); 
    gap_params_init( );
    gatt_init( );
    services_init( );
    advertising_init( );
    conn_params_init( );
    scan_init( );
}

void app_ble_advertising_start( void) 
{
    advertising_start( NULL );
}

void app_ble_advertising_stop( void )
{
    sd_ble_gap_adv_stop( m_advertising.adv_handle );
}

bool app_ble_is_disconnected( void )
{
    if( app_ble_state == BLE_GAP_EVT_DISCONNECTED ) return true;
    else if( app_ble_state == BLE_GAP_EVT_CONNECTED ) return false;
}

void app_ble_trace_print( const char* fmt, ... )
{
    va_list argp;
    va_start( argp, fmt );
    vprint( fmt, argp );
    va_end( argp );
}

void app_ble_disconnect( void )
{
    uint8_t i = 0;
    while( true )
    {
        if( !app_ble_is_disconnected( ))
        {
            disconnect( m_conn_handle, NULL );        
        }
        else
        {
            break;
        }
        hal_mcu_wait_us( 100 );
        i ++;
        if( i > 10 )
        {
            break;
        }
    }
}
