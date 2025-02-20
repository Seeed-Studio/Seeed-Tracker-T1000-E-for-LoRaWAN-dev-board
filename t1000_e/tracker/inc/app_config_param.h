
#ifndef __APP_CONFIG_PARAM_H
#define __APP_CONFIG_PARAM_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    NO_MODIFICATION = 0,
    DEVICE_INFO_CHANGE = ( 0x1UL << 0 )
} para_config_t;

typedef enum eRetryState
{
    RETRY_STATE_1C = 1,
    RETRY_STATE_1N = 2
} RetryState_t;

typedef enum
{
    IOT_PLATFORM_SENSECAP_TTN = 0,
    IOT_PLATFORM_OTHER,
    IOT_PLATFORM_HELIUM,
    IOT_PLATFORM_TTN,
    IOT_PLATFORM_SENSECAP_HELIUM,
    IOT_PLATFORM_MAX
} platform_t;

typedef enum eLoRaMacRegion
{
    LORAMAC_REGION_TTN_AS923_2 = 0, // TTN AS923_2
    LORAMAC_REGION_AU915,           // Australian band on 915MHz
    LORAMAC_REGION_CN470,           // Chinese band on 470MHz
    LORAMAC_REGION_CN779,           // Chinese band on 779MHz
    LORAMAC_REGION_EU433,           // European band on 433MHz
    LORAMAC_REGION_EU868,           // European band on 868MHz
    LORAMAC_REGION_KR920,           // South korean band on 920MHz
    LORAMAC_REGION_IN865,           // India band on 865MHz
    LORAMAC_REGION_US915,           // North american band on 915MHz
    LORAMAC_REGION_RU864,           // Russia band on 864MHz
    LORAMAC_REGION_AS923_1,         // Helium AS923_1
    LORAMAC_REGION_AS923_2,         // Helium AS923_2
    LORAMAC_REGION_AS923_3,         // Helium AS923_3
    LORAMAC_REGION_AS923_4,         // Helium AS923_4
    LORAMAC_REGION_AS923_1B,        // Helium AS923_1B
    LORAMAC_REGION_TTN_AS923_1,     // TTN AS923_1
    LORAMAC_REGION_AS_GP_2,         // AS923_GP_2
    LORAMAC_REGION_AS_GP_3,         // AS923_GP_3
    LORAMAC_REGION_AS_GP_4,         // AS923_GP_4
    LORAMAC_REGION_MAX = 0xFF       // none 
} LoRaMacRegion_t;

typedef enum eActivationType
{
    ACTIVATION_TYPE_ABP = 1,    // Activation By Personalization
    ACTIVATION_TYPE_OTAA = 2,   // Over-The-Air Activation
} ActivationType_t;

typedef struct lora_info
{
    platform_t Platform;        // 
    
    uint8_t ActiveRegion;       // Regional spectrum in LoRaMacRegion_t
    uint8_t ChannelGroup;       // Sub-band select, from 0 - 7, work for US915/AU915
    uint8_t ActivationType;     // Access net type 0: none; 1: ABP; 2: OTAA
    RetryState_t Retry;         // 1: 1C; 2: 1N

    uint8_t lr_ADR_en;
    uint8_t lr_DR_min;
    uint8_t lr_DR_max;

    uint8_t DevEui[8];
    uint8_t JoinEui[8];
    uint8_t AppKey[16];  

    uint32_t DevAddr;
    uint8_t NwkKey[16];
    uint8_t AppSKey[16];
    uint8_t NwkSKey[16];

    uint8_t DeviceCode[8]; 
    uint8_t DeviceKey[16];
} lora_info_t;

typedef struct hardware_info
{
    uint8_t Sn[9];

    uint8_t pos_strategy;       // positioning strategy
    uint16_t pos_interval;      // positioning interval
    uint8_t sos_mode;           // SOS mode
    uint8_t acc_en;             // Accelerometer switch

    uint16_t gnss_overtime;     // gnss scan timeout

    uint8_t wifi_max;           // wifi scan max number

    uint8_t beac_overtime;      // beacon scan timeout
    uint8_t beac_max;           // wifi scan max number
    uint8_t beac_uuid[16];      // 
    uint8_t uuid_num;           // 

    uint8_t test_mode;          // 
} hardware_info_t;

#pragma pack( 4 )

typedef struct app_param
{
    uint8_t param_version;
    lora_info_t lora_info;
    hardware_info_t hardware_info;
} app_param_t;

#pragma pack( )

extern app_param_t app_param;

void hexTonum( unsigned char *out_data, unsigned char *in_data, unsigned short Size );
void numTohex( unsigned char *out_data, unsigned char *in_data, unsigned short Size );
uint8_t Char2Nibble( char Char );

#ifdef __cplusplus
}
#endif

#endif
