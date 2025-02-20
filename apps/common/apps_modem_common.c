#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "apps_modem_common.h"
#include "apps_utilities.h"
#include "lorawan_key_config.h"
#include "smtc_modem_api.h"
#include "smtc_basic_modem_lr11xx_api_extension.h"
#include "smtc_board.h"
#include "smtc_hal.h"
#include "smtc_modem_api_str.h"

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE MACROS-----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE CONSTANTS -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE TYPES -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE VARIABLES -------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DECLARATION -------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC VARIABLES --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PRIVATE FUNCTIONS DEFINITION --------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS DEFINITION ---------------------------------------------
 */

void apps_modem_common_configure_lorawan_params( uint8_t stack_id )
{
    smtc_modem_return_code_t rc = SMTC_MODEM_RC_OK;
    uint8_t dev_eui[8] = { 0 };
    uint8_t join_eui[8]  = { 0 };
    uint8_t app_key[16] = { 0 };

    hal_hex_to_bin( LORAWAN_DEVICE_EUI, dev_eui, 8 );
    hal_hex_to_bin( LORAWAN_JOIN_EUI, join_eui, 8 );
    hal_hex_to_bin( LORAWAN_APP_KEY, app_key, 16 );

    rc = smtc_modem_set_deveui( stack_id, dev_eui );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_joineui( stack_id, join_eui );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_nwkkey( stack_id, app_key );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_nwkkey failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    HAL_DBG_TRACE_INFO( "LoRaWAN parameters:\n" );

    rc = smtc_modem_get_deveui( stack_id, dev_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "DevEUI", dev_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_get_deveui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_get_joineui( stack_id, join_eui );
    if( rc == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ARRAY( "JoinEUI", join_eui, SMTC_MODEM_EUI_LENGTH );
    }
    else
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_get_joineui failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    rc = smtc_modem_set_class( stack_id, LORAWAN_CLASS );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_class failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_class_to_string( LORAWAN_CLASS );

    if( LORAWAN_REGION == SMTC_MODEM_REGION_US_915 || LORAWAN_REGION == SMTC_MODEM_REGION_AU_915 )
    {
        rc = smtc_modem_set_region_sub_band( stack_id, 2 );
        if( rc != SMTC_MODEM_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_set_region_sub_band failed (%d)\n", rc );
        }
    }

    rc = smtc_modem_set_region( stack_id, LORAWAN_REGION );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_region failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_region_to_string( LORAWAN_REGION );

    /* adapt the tx power offet depending on the board */
    rc |= smtc_modem_set_tx_power_offset_db( stack_id, smtc_board_get_tx_power_offset( ) );
}

void apps_modem_common_display_lbm_version( void )
{
    smtc_modem_return_code_t     modem_response_code = SMTC_MODEM_RC_OK;
    smtc_modem_lorawan_version_t lorawan_version;
    smtc_modem_version_t         firmware_version;

    modem_response_code = smtc_modem_get_lorawan_version( &lorawan_version );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_INFO( "LoRaWAN version: %.2x.%.2x.%.2x.%.2x\n", lorawan_version.major, lorawan_version.minor,
                            lorawan_version.patch, lorawan_version.revision );
    }

    modem_response_code = smtc_modem_get_modem_version( &firmware_version );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_INFO( "LoRa Basics Modem version: %.2x.%.2x.%.2x\n", firmware_version.major,
                            firmware_version.minor, firmware_version.patch );
    }
}
