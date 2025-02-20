
#include "ralf.h"
#include "smtc_modem_hal.h"
#include "wifi_helpers.h"
#include "smtc_hal_dbg_trace.h"
#include "wifi_scan.h"

/**
 * @brief Size in bytes to store the RSSI of a detected WiFi Access-Point
 */
#define WIFI_AP_RSSI_SIZE ( 1 )

/**
 * @brief Size in bytes of a WiFi Access-Point address
 */
#define WIFI_AP_ADDRESS_SIZE ( 6 )

/*!
 * @brief Results of the current Wi-Fi scan
 */
wifi_scan_all_result_t wifi_results;

/*!
 * @brief The buffer containing results to be sent over the air
 */
uint8_t wifi_result_buffer[4+( WIFI_AP_RSSI_SIZE + WIFI_AP_ADDRESS_SIZE ) * WIFI_MAX_RESULTS + 4];

bool wifi_scan_start( ralf_t* modem_radio )
{
    wifi_settings_t wifi_settings = { 0 };

    /* Init settings */
    wifi_settings.channels            = 0x3FFF; /* all channels enabled */
    wifi_settings.types               = LR11XX_WIFI_TYPE_SCAN_B_G_N;
    wifi_settings.max_results         = WIFI_MAX_RESULTS;
    wifi_settings.timeout_per_channel = WIFI_TIMEOUT_PER_CHANNEL_DEFAULT;
    wifi_settings.timeout_per_scan    = WIFI_TIMEOUT_PER_SCAN_DEFAULT;
    smtc_wifi_settings_init( &wifi_settings );
    
    /* Start WIFI scan */
    if( smtc_wifi_start_scan( modem_radio->ral.context ) != true )
    {
        HAL_DBG_TRACE_PRINTF( "RP_TASK_WIFI - failed to start scan, abort task\n" );
        return false;
    }
    return true;
}

bool wifi_get_results( ralf_t* modem_radio, uint8_t* result, uint8_t *size )
{
    bool scan_results_rc;

    /* Reset previous results */
    memset( &wifi_results, 0, sizeof( wifi_results ));

    /* Wi-Fi scan completed, get and display the results */
    scan_results_rc = smtc_wifi_get_results( modem_radio->ral.context, &wifi_results );

    /* Get scan power consumption */
    smtc_wifi_get_power_consumption( modem_radio->ral.context, &wifi_results.power_consumption_uah );

    if( scan_results_rc == true )
    {
        if( wifi_results.nbr_results )
        {
            uint8_t wifi_buffer_size = 0;

            /* Concatenate all results in send buffer */
            for( uint8_t i = 0; i < wifi_results.nbr_results; i++ )
            {
                /* Copy Access Point MAC address in result buffer */
                memcpy( &wifi_result_buffer[wifi_buffer_size], wifi_results.results[i].mac_address, WIFI_AP_ADDRESS_SIZE );
                wifi_buffer_size += WIFI_AP_ADDRESS_SIZE;

                /* Copy Access Point RSSI address in result buffer (if requested) */
                wifi_result_buffer[wifi_buffer_size] = wifi_results.results[i].rssi;
                wifi_buffer_size += WIFI_AP_RSSI_SIZE;
            }

            // if( wifi_results.nbr_results < WIFI_MAX_RESULTS )
            // {
            //     for( uint8_t i = 0; i < ( WIFI_MAX_RESULTS - wifi_results.nbr_results ); i++  )
            //     {
            //         memset( &wifi_result_buffer[wifi_buffer_size], 0xff, ( WIFI_AP_ADDRESS_SIZE + WIFI_AP_RSSI_SIZE ));
            //         wifi_buffer_size += ( WIFI_AP_ADDRESS_SIZE + WIFI_AP_RSSI_SIZE );
            //     }
            // }
            
            if( result ) memcpy( result, wifi_result_buffer, wifi_buffer_size );
            if( size ) memcpy( size, &wifi_buffer_size, 1 );

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "RP_TASK_WIFI - unkown error on get results\n" );
        return false;
    }
}

void wifi_display_results( void )
{
    HAL_DBG_TRACE_PRINTF( "Number of results: %u\r\n", wifi_results.nbr_results );
    for( uint8_t i = 0; i < wifi_results.nbr_results; i++ )
    {
        for( uint8_t j = 0; j < WIFI_AP_ADDRESS_SIZE; j++ )
        {
            HAL_DBG_TRACE_PRINTF( "%02X ", wifi_results.results[i].mac_address[j] );
        }
        HAL_DBG_TRACE_PRINTF( "Channel: %d, ", wifi_results.results[i].channel );
        HAL_DBG_TRACE_PRINTF( "Type: %d, ", wifi_results.results[i].type );
        HAL_DBG_TRACE_PRINTF( "RSSI: %d\r\n", wifi_results.results[i].rssi );
    }
    HAL_DBG_TRACE_PRINTF( "\n" );
}

void wifi_scan_stop( ralf_t* modem_radio )
{
    lr11xx_system_sleep_cfg_t radio_sleep_cfg;
    radio_sleep_cfg.is_warm_start  = true;
    radio_sleep_cfg.is_rtc_timeout = false;
    if( lr11xx_system_cfg_lfclk( modem_radio->ral.context, LR11XX_SYSTEM_LFCLK_RC, true ) != LR11XX_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set LF clock\n" );
    }
    if( lr11xx_system_set_sleep( modem_radio->ral.context, radio_sleep_cfg, 0 ) != LR11XX_STATUS_OK )
    {
        HAL_DBG_TRACE_ERROR( "Failed to set the radio to sleep\n" );
    }
}
