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
#include "lr1mac_defs.h"

#include "app_config_param.h"

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
    
    uint8_t lorawan_region = SMTC_MODEM_REGION_EU_868; // can be change by user
    uint8_t lorawan_region_sub_band = 2; // can be change by user
    uint8_t activation_mode = ACTIVATION_MODE_OTAA; // can be change by user
    uint32_t dev_addr = 0; // can be change by user
    uint8_t dev_eui[8] = { 0 }; // can be change by user
    uint8_t join_eui[8] = { 0 }; // can be change by user
    uint8_t app_key[16] = { 0 }; // can be change by user
    uint8_t app_s_key[16] = { 0 }; // can be change by user
    uint8_t nwk_s_key[16] = { 0 }; // can be change by user

    switch( app_param.lora_info.ActiveRegion )
    {
        case 0:
        case 15:
            lorawan_region = SMTC_MODEM_REGION_AS_923_GRP1;
        break;

        case 16:
            lorawan_region = SMTC_MODEM_REGION_AS_923_GRP2;
        break;

        case 17:
            lorawan_region = SMTC_MODEM_REGION_AS_923_GRP3;
        break;

        case 18:
            lorawan_region = SMTC_MODEM_REGION_AS_923_GRP4;
        break;
        
        case 1:
            lorawan_region = SMTC_MODEM_REGION_AU_915;
        break;
        
        case 5:
            lorawan_region = SMTC_MODEM_REGION_EU_868;
        break;
        
        case 6:
            lorawan_region = SMTC_MODEM_REGION_KR_920;
        break;
    
        case 7:
            lorawan_region = SMTC_MODEM_REGION_IN_865;
        break;
        
        case 8:
            lorawan_region = SMTC_MODEM_REGION_US_915;
        break;
    
        case 9:
            lorawan_region = SMTC_MODEM_REGION_RU_864;
        break;
        
        case 10:
            lorawan_region = SMTC_MODEM_REGION_AS_923_HELIUM_1;
        break;
        
        case 11:
            lorawan_region = SMTC_MODEM_REGION_AS_923_HELIUM_2;
        break;
        
        case 12:
            lorawan_region = SMTC_MODEM_REGION_AS_923_HELIUM_3;
        break;
        
        case 13:
            lorawan_region = SMTC_MODEM_REGION_AS_923_HELIUM_4;
        break;
        
        case 14:
            lorawan_region = SMTC_MODEM_REGION_AS_923_HELIUM_1B;
        break;
        
        default:
        break;
    }
    
    lorawan_region_sub_band =  app_param.lora_info.ChannelGroup + 1;
    
    switch( app_param.lora_info.ActivationType )
    {
        case 1:
            activation_mode = ACTIVATION_MODE_ABP;
        break;
        
        case 2:
            activation_mode = ACTIVATION_MODE_OTAA;
        break;
        
        default:
        break;
    }
    
    dev_addr = app_param.lora_info.DevAddr;
    memcpy1( dev_eui, app_param.lora_info.DevEui, 8 );
    memcpy1( join_eui, app_param.lora_info.JoinEui, 8 );
    memcpy1( app_key, app_param.lora_info.AppKey, 16 );
    memcpy1( app_s_key, app_param.lora_info.AppSKey, 16 );
    memcpy1( nwk_s_key, app_param.lora_info.NwkSKey, 16 );

    if( activation_mode == ACTIVATION_MODE_OTAA )
    {
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
    }
    else if( activation_mode == ACTIVATION_MODE_ABP )
    {
        rc = smtc_modem_set_devaddr( stack_id, dev_addr );
        if( rc != SMTC_MODEM_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_set_devaddr failed (%d)\n", rc );
        }
        
        rc = smtc_modem_set_appskey( stack_id, app_s_key );
        if( rc != SMTC_MODEM_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_set_appskey failed (%d)\n", rc );
        }
        
        rc = smtc_modem_set_nwkskey( stack_id, nwk_s_key );
        if( rc != SMTC_MODEM_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_set_nwkskey failed (%d)\n", rc );
        }
    }

    rc = smtc_modem_set_class( stack_id, SMTC_MODEM_CLASS_A );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_class failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }

    modem_class_to_string( SMTC_MODEM_CLASS_A );

    if( lorawan_region == SMTC_MODEM_REGION_US_915 || lorawan_region == SMTC_MODEM_REGION_AU_915 )
    {
        rc = smtc_modem_set_region_sub_band( stack_id, lorawan_region_sub_band );
        if( rc != SMTC_MODEM_RC_OK )
        {
            HAL_DBG_TRACE_ERROR( "smtc_modem_set_region_sub_band failed (%d)\n", rc );
        }
    }

    uint8_t lorawan_region_save;
    smtc_modem_get_region( stack_id, &lorawan_region_save );

    rc = smtc_modem_set_region( stack_id, lorawan_region );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_region failed: rc=%s (%d)\n", smtc_modem_return_code_to_str( rc ), rc );
    }
    
    modem_region_to_string( lorawan_region );

    if( lorawan_region != lorawan_region_save )
    {
        /*hal_device_log_error( "reboot and reload lbm param", 0 );
        hal_mcu_wait_ms( 50 );
        hal_mcu_reset( ); // reboot and reload lbm param
        */
    }

    rc = smtc_modem_set_activation_mode( stack_id, activation_mode );
    if( rc != SMTC_MODEM_RC_OK )
    {
        HAL_DBG_TRACE_ERROR( "smtc_modem_set_activation_mode failed (%d)\n", rc );
    }

    if( activation_mode == ACTIVATION_MODE_ABP )
    {
        set_modem_status_modem_joined( true );
    }

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
