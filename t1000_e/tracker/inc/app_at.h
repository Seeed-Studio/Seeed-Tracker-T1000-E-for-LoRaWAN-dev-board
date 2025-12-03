
#ifndef __APP_AT_H
#define __APP_AT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t at_config_flag;
extern uint8_t parse_cmd_type;

extern void hal_trace_print_var( const char* fmt, ... );
extern void app_ble_trace_print( const char* fmt, ... );

#define AT_PRINTF(...)     do{   if(parse_cmd_type == 1)\
                                 {\
                                    app_ble_trace_print (  __VA_ARGS__ );\
                                 }\
                                 else\
                                 {\
                                    hal_trace_print_var (  __VA_ARGS__ );\
                                 }\
                                 hal_mcu_wait_ms( 1 );\
                             }while(0)

/*
 * AT Command Id errors. Note that they are in sync with ATError_description static array
 * in command.c
 */
typedef enum eATEerror
{
    AT_OK = 0,
    AT_ERROR,
    AT_PARAM_ERROR,
    AT_BUSY_ERROR,
    AT_TEST_PARAM_OVERFLOW,
    AT_NO_NET_JOINED,
    AT_RX_ERROR,
    AT_NO_CLASS_B_ENABLE,
    AT_DUTYCYCLE_RESTRICTED,
    AT_CRYPTO_ERROR,
    AT_SAVE_FAILED,
    AT_READ_ERROR,
    AT_MAX,
} ATEerror_t;

/* AT Command strings. Commands start with AT */
/* General */
#define AT_RESET            "Z"
#define AT_VER              "+VER"
#define AT_CONFIG           "+CONFIG"
#define AT_SN               "+SN"

/* LoRaWAN network */
#define AT_TYPE             "+TYPE"       
#define AT_BAND             "+BAND"       
#define AT_CHANNEL          "+CHANNEL"  
#define AT_CLASS            "+CLASS"   
#define AT_RETEY            "+RETRY" 
#define AT_PLATFORM         "+PLATFORM"

#define AT_LR_ADR_EN        "+LR_ADR_EN"
#define AT_LR_DR_MIN        "+LR_DR_MIN"
#define AT_LR_DR_MAX        "+LR_DR_MAX"

/* Keys */
#define AT_JOINEUI          "+APPEUI"      
#define AT_NWKKEY           "+NWKKEY"      
#define AT_APPKEY           "+APPKEY"
#define AT_NWKSKEY          "+NWKSKEY"     
#define AT_APPSKEY          "+APPSKEY"     
#define AT_DADDR            "+DADDR"       
#define AT_DEUI             "+DEUI"
#define AT_DCODE            "+DCODE" 
#define AT_DKEY             "+DKEY"

/* Basic */
#define AT_POS_STRATEGY     "+POS_STRATEGY"
#define AT_POS_INT          "+POS_INT"
#define AT_SOS_MODE         "+SOS_MODE"
#define AT_ACC_EN           "+ACC_EN"

/* GNSS */
#define AT_STA_OT           "+STA_OT"

/* Wifi */
#define AT_WIFI_MAX         "+WIFI_MAX"

/* iBeacon */
#define AT_BEAC_MAX         "+BEAC_MAX"
#define AT_BEAC_OT          "+BEAC_OT"
#define AT_BEAC_UUID        "+BEAC_UUID"

/* Other */
#define AT_MEA              "+MEA"
#define AT_BAT              "+BAT"
#define AT_BUZ_EN           "+BUZ_EN"
#define AT_TESTMODE_TYPE    "+TESTMODE_TYPE"
#define AT_DISCONNECT       "+DISCONNECT"      
#define AT_LBDADDR          "+LBDADDR"  
#define AT_POWER_OFF        "+POWER_OFF"


/**
  * @brief  Return AT_OK in all cases
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_return_ok(const char *param);

/**
  * @brief  Return AT_ERROR in all cases
  * @param  param string of the AT command - unused
  * @retval AT_ERROR
  */
ATEerror_t AT_return_error(const char *param);

/* --------------- General commands --------------- */
/**
  * @brief  Trig a reset of the MCU
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_reset(const char *param);

/* --------------- Keys, IDs and EUIs management commands --------------- */
/**
  * @brief  Print Join Eui
  * @param  param string of the AT command
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_JoinEUI_get(const char *param);

/**
  * @brief  Set Join Eui
  * @param  param string of the AT command
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_JoinEUI_set(const char *param);

/**
  * @brief  Print Network Key
  * @param  param string of the AT command
  * @retval AT_OK
  */
ATEerror_t AT_NwkKey_get(const char *param);

/**
  * @brief  Set Network Key
  * @param  param string of the AT command
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_NwkKey_set(const char *param);

/**
  * @brief  Print Application Key
  * @param  param string of the AT command
  * @retval AT_OK
  */
ATEerror_t AT_AppKey_get(const char *param);

/**
  * @brief  Set Application Key
  * @param  param string of the AT command
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_AppKey_set(const char *param);

/**
  * @brief  Print Network Session Key
  * @param  param string of the AT command
  * @retval AT_OK
  */
ATEerror_t AT_NwkSKey_get(const char *param);

/**
  * @brief  Set Network Session Key
  * @param  param String pointing to provided DevAddr
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_NwkSKey_set(const char *param);

/**
  * @brief  Print Application Session Key
  * @param  param string of the AT command
  * @retval AT_OK
  */
ATEerror_t AT_AppSKey_get(const char *param);

/**
  * @brief  Set Application Session Key
  * @param  param String pointing to provided DevAddr
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_AppSKey_set(const char *param);

/**
  * @brief  Print the DevAddr
  * @param  param String pointing to provided DevAddr
  * @retval AT_OK
  */
ATEerror_t AT_DevAddr_get(const char *param);

/**
  * @brief  Set DevAddr
  * @param  param String pointing to provided DevAddr
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */
ATEerror_t AT_DevAddr_set(const char *param);

/**
  * @brief  Print Device EUI
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_DevEUI_get(const char *param);

/**
  * @brief  Set Device EUI
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_DevEUI_set(const char *param);

/**
  * @brief  Print the version of the AT_Slave FW
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_version_get(const char *param);

/**
  * @brief  Print actual Active Region
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_Region_get(const char *param);

/**
  * @brief  Set Active Region
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_Region_set(const char *param);

/**
  * @brief  Print actual Active Type
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_ActivationType_get(const char *param);

/**
  * @brief  Set Active Type
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_ActivationType_set(const char *param);

/**
  * @brief  Print channel group
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_ChannelGroup_get(const char *param);

/**
  * @brief  Set channel group
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_ChannelGroup_set(const char *param);

/**
  * @brief  Set Sensor Type
  * @param  param String pointing to provided param
  * @retval AT_OK if OK, or an appropriate AT_xxx error code
  */

ATEerror_t AT_DeviceClass_get(const char *param);

/**
  * @brief  get enable/disable LoRaWan ADR
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_LR_ADR_EN_get(const char *param);

/**
  * @brief  Set enable/disable LoRaWan ADR
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_LR_ADR_EN_set(const char *param);

/**
  * @brief  get minimum LoRa data rate
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_LR_DR_MIN_get(const char *param);

/**
  * @brief  Set minimum LoRa data rate when disable LoRaWan ADR
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_LR_DR_MIN_set(const char *param);

/**
  * @brief  get maximum LoRa data rate
  * @param  param string of the AT command - unused
  * @retval AT_OK
  */
ATEerror_t AT_LR_DR_MAX_get(const char *param);

/**
  * @brief  Set maximum LoRa data rate when disable LoRaWan ADR
  * @param  param string of the AT command
  * @retval AT_OK if OK
  */
ATEerror_t AT_LR_DR_MAX_set(const char *param);

/**
  * @brief  collect sensor data
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_MeasurementValue_get(const char *param);

/**
  * @brief  disconnect ble connection
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Disconnect(const char *param);

/**
  * @brief  set lora ack type
  * @param  param String parameter
  * @retval AT_OK
  */

ATEerror_t AT_Retry_set(const char *param);

/**
  * @brief  get lora ack type
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Retry_get(const char *param);

/**
  * @brief  get device paramenter
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Config_get(const char *param);

/**
  * @brief  set device code
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_DevCODE_set(const char *param);

/**
  * @brief  get device code
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_DevCODE_get(const char *param);

/**
  * @brief  set lora join net platform
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Platform_set(const char *param);

/**
  * @brief  get lora join net platform
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Platform_get(const char *param);

/**
  * @brief  set device serial number
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Sn_set(const char *param);

/**
  * @brief  get device serial number
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_Sn_get(const char *param);

/**
  * @brief  set lora device key
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_DeviceKey_set(const char *param);

/**
  * @brief  get lora device key
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_DeviceKey_get(const char *param);

/**
  * @brief  get ble mac address
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BluetoothMacAddr_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_TESTMODE_TYPE_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_TESTMODE_TYPE_set(const char *param);

/**
  * @brief  Get the battery level  unit:mV
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_bat_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_POS_STRATEGY_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_POS_STRATEGY_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_POS_INT_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_POS_INT_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_SOS_MODE_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_SOS_MODE_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_ACC_EN_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_ACC_EN_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_STA_OT_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_STA_OT_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BUZ_EN_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BUZ_EN_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_OT_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_OT_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_UUID_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_UUID_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_MAX_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_BEAC_MAX_set(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_WIFI_MAX_get(const char *param);

/**
  * @brief  
  * @param  param String parameter
  * @retval AT_OK
  */
ATEerror_t AT_WIFI_MAX_set(const char *param);

/**
 * @brief Power off the device
 * 
 * @param param 
 * @return ATEerror_t 
 */
ATEerror_t AT_POWER_OFF_run(const char *param);

#ifdef __cplusplus
}
#endif

#endif
