#include "nordic_common.h"
#include "app_error.h"
#include "nrf_sdm.h"
#include "ble.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_scan.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "smtc_hal.h"
#include "ble_scan.h"

#define APP_SCAN_INTERVAL   160         /**< Determines scan interval(in units of 0.625 ms). */
#define APP_SCAN_WINDOW     80          /**< Determines scan window(in units of 0.625 ms). */
#define APP_SCAN_DURATION   0           /**< Duration of the scanning(in units of 10 ms). */

#define APP_BLE_CONN_CFG_TAG      1     /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO     3     /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_SOC_OBSERVER_PRIO     1     /**< Applications' SoC observer priority. You shouldn't need to modify this value. */

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

void ble_scan_init( void )
{
    ble_stack_init( );
    scan_init( );
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

        if( beacon_num < BLE_BEACON_SEND_MUM )
        {
            for( uint8_t i = 0; i < ( BLE_BEACON_SEND_MUM - beacon_num ); i++ )
            {
                memset( result + beacon_num * 7 + i * 7, 0xff, 7 );
                *size += 7;
            }
        }
        
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
