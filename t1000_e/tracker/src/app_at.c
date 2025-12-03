
#include "apps_utilities.h"
#include "smtc_modem_api.h"
#include "smtc_hal.h"
#include "app_at.h"
#include "app_board.h"
#include "app_at_command.h"
#include "app_config_param.h"
#include "app_at_fds_datas.h"

#define tiny_sscanf sscanf

#define SE_KEY_SIZE SMTC_MODEM_KEY_LENGTH
#define SE_EUI_SIZE SMTC_MODEM_EUI_LENGTH

uint8_t at_config_flag = NO_MODIFICATION;
uint8_t parse_cmd_type = 0;

/**
 * @brief  Print 4 bytes as %02x
 * @param  value containing the 4 bytes to print
 */
static void print_uint32_as_02x(uint32_t value);

/**
 * @brief  Print 16 bytes as %02X
 * @param  pt pointer containing the 16 bytes to print
 */
static void print_16_02x(uint8_t *pt);

/**
 * @brief  Print 8 bytes as %02X
 * @param  pt pointer containing the 8 bytes to print
 */
static void print_8_02x(uint8_t *pt);

/**
 * @brief  Print an int
 * @param  value to print
 */
static void print_d(int32_t value);

/**
 * @brief  Print an unsigned int
 * @param  value to print
 */
static void print_u(uint32_t value);

/**
 * @brief  Check if character in parameter is alphanumeric
 * @param  Char for the alphanumeric check
 */
static int32_t isHex(char Char);

/**
 * @brief  Check if character in parameter is number
 * @param  Char for the number check
 */
static int32_t isNum(char Char); 

/**
 * @brief  Check if character in parameter is number
 * @param  str for the number check
 * @param  Size for the number size we want check
 */
int32_t isNums(const char *str, uint32_t Size);

/**
 * @brief  Converts hex string to a nibble ( 0x0X )
 * @param  Char hex string
 * @retval the nibble. Returns 0xF0 in case input was not an hex string (a..f; A..F or 0..9)
 */
// static uint8_t Char2Nibble(char Char);

/**
 * @brief  Convert a string into a buffer of data
 * @param  str string to convert
 * @param  data output buffer
 * @param  Size of input string
 */
static int32_t stringToData(const char *str, uint8_t *data, uint32_t Size);

static int8_t ascii_4bit_to_hex(uint8_t ascii) {
    int8_t result = -1;
    if ((ascii >= '0') && (ascii <= '9')) {
        result = ascii - 0x30;
    } else if ((ascii >= 'a') && (ascii <= 'f')) {
        result = ascii - 'a' + 10;
    } else if ((ascii >= 'A') && (ascii <= 'F')) {
        result = ascii - 'A' + 10;
    }
    return result;
}

static void uint64_to_bytes(uint64_t data,uint8_t *bytes) {
    bytes[0] = (uint8_t)(data>>56);
    bytes[1] = (uint8_t)(data>>48);
    bytes[2] = (uint8_t)(data>>40);
    bytes[3] = (uint8_t)(data>>32);
    bytes[4] = (uint8_t)(data>>24);
    bytes[5] = (uint8_t)(data>>16);
    bytes[6] = (uint8_t)(data>>8);
    bytes[7] = (uint8_t)(data);
}

static int8_t Data_Analysis(const char *param, uint8_t Buff[], uint8_t lenth) {
    uint8_t param_length = 0;
    int8_t result = 0;
    for (uint8_t j = 0; j < lenth; j++) {
        if (param_length > strlen(param)) {
            return -1;
        }
        result = ascii_4bit_to_hex(param[param_length++]);
        if (result == -1) {
            return -1;
        } else {
            Buff[j] = result << 4;
        }
        result = ascii_4bit_to_hex(param[param_length++]);
        if (result == -1) {
            return -1;
        } else {
            Buff[j] = Buff[j] | result;
        }
        param_length++; // The byte is a colon
    }
    return 0;
}

static void print_uint32_as_02x(uint32_t value) 
{
    AT_PRINTF("\"%02X:%02X:%02X:%02X\"",
        (unsigned)((unsigned char *)(&value))[3],
        (unsigned)((unsigned char *)(&value))[2],
        (unsigned)((unsigned char *)(&value))[1],
        (unsigned)((unsigned char *)(&value))[0]);
}

static void print_16_02x(uint8_t *pt) 
{
    AT_PRINTF("\"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\"",
        pt[0], pt[1], pt[2], pt[3],
        pt[4], pt[5], pt[6], pt[7],
        pt[8], pt[9], pt[10], pt[11],
        pt[12], pt[13], pt[14], pt[15]);
}

static void print_8_02x(uint8_t *pt) 
{
    AT_PRINTF("\"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\"",
        pt[0], pt[1], pt[2], pt[3], pt[4], pt[5], pt[6], pt[7]);
}

static void print_d(int32_t value) 
{
    AT_PRINTF("%d\r\n", value);
}

static void print_u(uint32_t value) 
{
    AT_PRINTF("%u\r\n", value);
}

static int32_t stringToData(const char *str, uint8_t *data, uint32_t Size) 
{
    char hex[3];
    hex[2] = 0;
    int32_t ii = 0;
    while (Size-- > 0) 
    {
        hex[0] = *str++;
        hex[1] = *str++;

        /*check if input is hex */
        if ((isHex(hex[0]) == -1) || (isHex(hex[1]) == -1)) 
        {
            return -1;
        }
        /*check if input is even nb of character*/
        if ((hex[1] == '\0') || (hex[1] == ',')) 
        {
            return -1;
        }
        data[ii] = (Char2Nibble(hex[0]) << 4) + Char2Nibble(hex[1]);
        ii++;
    }

    return 0;
}

static int32_t isHex(char Char) 
{
    if (((Char >= '0') && (Char <= '9')) ||
        ((Char >= 'a') && (Char <= 'f')) ||
        ((Char >= 'A') && (Char <= 'F'))) 
    {
        return 0;
    } 
    else
    {
        return -1;
    }
}

static int32_t isNum(char Char) 
{
    if ((Char >= '0') && (Char <= '9'))
    {
        return 0;
    } 
    else
    {
        return -1;
    }
}

int32_t isNums(const char *str, uint32_t Size) 
{
    for(uint8_t u8i; u8i < Size; u8i++)
    {
        /*check if input is hex */
        if ((isNum(str[u8i]) == -1)) 
        {
            return -1;
        }
    }
    return 0;
}

ATEerror_t AT_return_error(const char *param) 
{
    return AT_ERROR;
}

/*------------------------ATZ\r\n-------------------------------------*/
ATEerror_t AT_reset(const char *param) 
{
    AT_PRINTF("\r\nOK\r\n");
    hal_mcu_wait_ms( 100 );      
    hal_mcu_reset();  
    return AT_OK;    
}
/*------------------------ATZ\r\n-------------------------------------*/

/*------------------------AT+APPEUI=?\r\n-------------------------------------*/
ATEerror_t AT_JoinEUI_get(const char *param) {
    print_8_02x(app_param.lora_info.JoinEui);
    return AT_OK;
}

ATEerror_t AT_JoinEUI_set(const char *param) {
    uint8_t joineui[8];
    if ((Data_Analysis(param, joineui, 8) == -1) || (strlen(param) != 25)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.JoinEui[0],joineui,SE_EUI_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.JoinEui, joineui, SE_EUI_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+APPEUI=?\r\n-------------------------------------*/

/*------------------------AT+NWKKEY=?\r\n-------------------------------------*/
ATEerror_t AT_NwkKey_get(const char *param) {
    print_16_02x(app_param.lora_info.NwkKey);

    return AT_OK;
}

ATEerror_t AT_NwkKey_set(const char *param) {
    uint8_t nwkKey[16];

    if ((Data_Analysis(param, nwkKey, 16) == -1) || (strlen(param) != 49)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.NwkKey[0],nwkKey,SE_KEY_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.NwkKey, nwkKey, SE_KEY_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+NWKKEY=?\r\n-------------------------------------*/

/*------------------------AT+APPKEY=?\r\n-------------------------------------*/
ATEerror_t AT_AppKey_get(const char *param) {
    print_16_02x(app_param.lora_info.AppKey);
    return AT_OK;
}

ATEerror_t AT_AppKey_set(const char *param) {
    uint8_t appKey[16];
    if ((Data_Analysis(param, appKey, 16) == -1) || (strlen(param) != 49)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.AppKey[0], appKey, SE_KEY_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.AppKey, appKey, SE_KEY_SIZE);
        check_save_param_type();   
    }
    return AT_OK;
}
/*------------------------AT+APPKEY=?\r\n-------------------------------------*/

/*------------------------AT+NWKSKEY=?\r\n-------------------------------------*/

ATEerror_t AT_NwkSKey_get(const char *param) {
    print_16_02x(app_param.lora_info.NwkSKey);
    return AT_OK;
}

ATEerror_t AT_NwkSKey_set(const char *param) {
    uint8_t nwkSKey[16];
    if ((Data_Analysis(param, nwkSKey, 16) == -1) || (strlen(param) != 49)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.NwkSKey[0], nwkSKey, 16) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.NwkSKey, nwkSKey, 16);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+NWKSKEY=?\r\n-------------------------------------*/

/*------------------------AT+APPSKEY=?\r\n -------------------------------------*/
ATEerror_t AT_AppSKey_get(const char *param) {
    print_16_02x(app_param.lora_info.AppSKey);

    return AT_OK;
}

ATEerror_t AT_AppSKey_set(const char *param) {
    uint8_t appskey[16];
    if ((Data_Analysis(param, appskey, 16) == -1) || (strlen(param) != 49)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.AppSKey[0], appskey, SE_KEY_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.AppSKey, appskey, SE_KEY_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+APPSKEY=?\r\n -------------------------------------*/

/*------------------------AT+DADDR=?\r\n -------------------------------------*/
ATEerror_t AT_DevAddr_get(const char *param) {
    print_uint32_as_02x(app_param.lora_info.DevAddr);
    return AT_OK;
}

ATEerror_t AT_DevAddr_set(const char *param) {
    uint32_t devAddr;
    uint8_t devAddrtmp[4];
    if ((Data_Analysis(param, devAddrtmp, 4) == -1) || (strlen(param) != 13)) {
        return AT_PARAM_ERROR;
    }
    devAddr = (devAddrtmp[0] << 24) | (devAddrtmp[1] << 16) | (devAddrtmp[2] << 8) | devAddrtmp[3];
    if(app_param.lora_info.DevAddr != devAddr)
    {
        app_param.lora_info.DevAddr = devAddr;
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+DADDR=?\r\n -------------------------------------*/

/*------------------------AT+DEUI=?\r\n-------------------------------------*/
ATEerror_t AT_DevEUI_get(const char *param) {
    print_8_02x(app_param.lora_info.DevEui);
    return AT_OK;
}

ATEerror_t AT_DevEUI_set(const char *param) {
    uint8_t devEui[8];
    if ((Data_Analysis(param, devEui, 8) == -1) || (strlen(param) != 25)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.DevEui[0], devEui, SE_EUI_SIZE)!= 0)
    {
        memcpy((uint8_t *)app_param.lora_info.DevEui, devEui, SE_EUI_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+DEUI=?\r\n-------------------------------------*/

/*------------------------AT+VER=?\r\n-------------------------------------*/
ATEerror_t AT_version_get(const char *param) {
    smtc_modem_return_code_t modem_response_code = SMTC_MODEM_RC_OK;
    smtc_modem_lorawan_version_t lorawan_version;
    BoardVersion_t  BoardVersion;
    BoardVersion = smtc_board_version_get(  );
    AT_PRINTF("\r\n{\r\n\t\"sw_ver\": \"V%d.%d\",", BoardVersion.Fields.SwMajor,BoardVersion.Fields.SwMinor);
    AT_PRINTF("\r\n\t\"hw_ver\": \"V%X.%X\",", BoardVersion.Fields.HwMajor, BoardVersion.Fields.HwMinor); 
    modem_response_code = smtc_modem_get_lorawan_version( &lorawan_version );
    if( modem_response_code == SMTC_MODEM_RC_OK )
    {
        AT_PRINTF("\r\n\t\"LoRaWAN\": \"V%X.%X.%X\"\r\n}", lorawan_version.major, lorawan_version.minor, lorawan_version.patch);
    }
    return AT_OK;
}
/*------------------------AT+VER=?\r\n-------------------------------------*/

/*------------------------AT+BAND=?\r\n-------------------------------------*/
ATEerror_t AT_Region_get(const char *param) {
    if (app_param.lora_info.ActiveRegion > LORAMAC_REGION_AS_GP_4 && app_param.lora_info.ActiveRegion != 0xFF) {
        return AT_PARAM_ERROR;
    }
    AT_PRINTF("%d", app_param.lora_info.ActiveRegion);
    return AT_OK;
}

ATEerror_t AT_Region_set(const char *param) {
    LoRaMacRegion_t region;
    if (tiny_sscanf(param, "%hhu", &region) != 1) {
        return AT_PARAM_ERROR;
    }

    if (((region > LORAMAC_REGION_AS_GP_4)&& (region != LORAMAC_REGION_MAX)) || (region == LORAMAC_REGION_CN470) || (region == LORAMAC_REGION_CN779) || (region == LORAMAC_REGION_EU433)) {
        return AT_PARAM_ERROR;
    }

    if(app_param.lora_info.ActiveRegion != region)
    {
        app_param.lora_info.ActiveRegion = region;
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+BAND=?\r\n-------------------------------------*/

/*------------------------AT+TYPE=?\r\n-------------------------------------*/
ATEerror_t AT_ActivationType_get(const char *param) {
    if (app_param.lora_info.ActivationType > ACTIVATION_TYPE_OTAA) {
        return AT_PARAM_ERROR;
    }
    AT_PRINTF("%d", app_param.lora_info.ActivationType);
    return AT_OK;
}

ATEerror_t AT_ActivationType_set(const char *param) {
    uint8_t activetype;
    if (tiny_sscanf(param, "%hhu", &activetype) != 1) {
        return AT_PARAM_ERROR;
    }
    if (activetype > ACTIVATION_TYPE_OTAA) {
        return AT_PARAM_ERROR;
    }
    if(app_param.lora_info.ActivationType != activetype)
    {
        app_param.lora_info.ActivationType = activetype == ACTIVATION_TYPE_OTAA ? ACTIVATION_TYPE_OTAA : ACTIVATION_TYPE_ABP;
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+TYPE=?\r\n -------------------------------------*/

/*------------------------AT+CHANNEL=?\r\n -------------------------------------*/
ATEerror_t AT_ChannelGroup_get(const char *param) {
    AT_PRINTF("%d", app_param.lora_info.ChannelGroup);
    return AT_OK;
}

ATEerror_t AT_ChannelGroup_set(const char *param) {
    uint8_t channelgroup = 0;
    if (tiny_sscanf(param, "%hhu", &channelgroup) != 1) {
        return AT_PARAM_ERROR;
    }
    if ((channelgroup <= 7) && ((app_param.lora_info.ActiveRegion == LORAMAC_REGION_US915) || (app_param.lora_info.ActiveRegion == LORAMAC_REGION_AU915))) {
        if(app_param.lora_info.ChannelGroup != channelgroup)
        {
            app_param.lora_info.ChannelGroup = channelgroup;
            check_save_param_type();
        }
        return AT_OK;
    } 
    else 
    {
        return AT_PARAM_ERROR;
    }
}
/*------------------------AT+CHANNEL=?\r\n -------------------------------------*/

/*------------------------AT+CLASS=?\r\n -------------------------------------*/
ATEerror_t AT_DeviceClass_get(const char *param) {
    AT_PRINTF("\"A\"");
    return AT_OK;
}
/*------------------------AT+CLASS=?\r\n -------------------------------------*/

/*------------------------AT+CONFIG=?\r\n -------------------------------------*/
ATEerror_t AT_Config_get(const char *param) {
    AT_PRINTF("\r\n{\r\n");    
    
    AT_PRINTF("\t\"devMdl\": \"Tracker T1000-E\"");
    
    AT_PRINTF(",\r\n\t\"deviceEui\": ");
    if (AT_DevEUI_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"appEui\": ");
    if (AT_JoinEUI_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"version\": ");
    if (AT_version_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"classType\": ");
    if (AT_DeviceClass_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"frequency\": ");
    if (AT_Region_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"subBand\": ");
    if (AT_ChannelGroup_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"3c\": ");
    if (AT_Retry_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"joinType\": ");
    if (AT_ActivationType_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"appKey\": ");
    if (AT_AppKey_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"nwkSkey\": ");
    if (AT_NwkSKey_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"appSkey\": ");
    if (AT_AppSKey_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"devAddr\": ");
    if (AT_DevAddr_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"platform\": ");
    if (AT_Platform_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"devCode\": ");
    if (AT_DevCODE_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"devKey\": ");
    if (AT_DeviceKey_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"lrAdrEn\": ");
    if (AT_LR_ADR_EN_get(param) != AT_OK) {
        return AT_ERROR;
    }
    AT_PRINTF(",\r\n\t\"lrDrMin\": ");
    if (AT_LR_DR_MIN_get(param) != AT_OK) {
        return AT_ERROR;
    }    
    AT_PRINTF(",\r\n\t\"lrDrMax\": ");
    if (AT_LR_DR_MAX_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"posStrategy\": ");
    if (AT_POS_STRATEGY_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"posInt\": ");
    if (AT_POS_INT_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"sosMode\": ");
    if (AT_SOS_MODE_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"accEn\": ");
    if (AT_ACC_EN_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"staOt\": ");
    if (AT_STA_OT_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"beacOt\": ");
    if (AT_BEAC_OT_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"beacUuid\": ");
    if (AT_BEAC_UUID_get(param) != AT_OK) {
        return AT_ERROR;
    }
    
    AT_PRINTF(",\r\n\t\"beacMax\": ");
    if (AT_BEAC_MAX_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"wifiMax\": ");
    if (AT_WIFI_MAX_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"batPct\": ");
    if (AT_bat_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF(",\r\n\t\"testMode\": ");
    if (AT_TESTMODE_TYPE_get(param) != AT_OK) {
        return AT_ERROR;
    }

    AT_PRINTF("\r\n}");
    return AT_OK;
}
/*------------------------AT+CONFIG=?\r\n -------------------------------------*/

/*------------------------AT+DCODE=?\r\n-------------------------------------*/
ATEerror_t AT_DevCODE_get(const char *param) {
    print_8_02x(app_param.lora_info.DeviceCode);
    return AT_OK;
}

ATEerror_t AT_DevCODE_set(const char *param) {
    uint8_t devCode[8];
    if ((Data_Analysis(param, devCode, 8) == -1) || (strlen(param) != 25)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.DeviceCode[0], devCode, SE_EUI_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.DeviceCode, devCode, SE_EUI_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+DCODE=?\r\n-------------------------------------*/

/*------------------------AT+PLATFORM=?\r\n-------------------------------------*/
ATEerror_t AT_Platform_get(const char *param) {
    AT_PRINTF("%d", (int32_t)(app_param.lora_info.Platform));

    return AT_OK;
}

ATEerror_t AT_Platform_set(const char *param) {
    uint8_t platform = 0;
    if (tiny_sscanf(param, "%hhu", &platform) != 1) {
        return AT_PARAM_ERROR;
    }
    
    if (platform < IOT_PLATFORM_MAX) {
        if(app_param.lora_info.Platform != platform)
        {
            app_param.lora_info.Platform = (platform_t)platform;
            check_save_param_type();
        }
        return AT_OK;
    } else {
        return AT_PARAM_ERROR;
    }
}
/*------------------------AT+PLATFORM=?\r\n-------------------------------------*/

/*------------------------AT+LR_ADR_EN=?\r\n-------------------------------------*/
ATEerror_t AT_LR_ADR_EN_get(const char *param) {
    AT_PRINTF( "%d",app_param.lora_info.lr_ADR_en==true?1:0 );     
    return AT_OK;
}
ATEerror_t AT_LR_ADR_EN_set(const char *param) {
    uint8_t enable_temp=0;
    bool b_en = false;
    if (1 != tiny_sscanf(param, "%hhu", &enable_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if(strlen(param) != 3 || enable_temp > 1)
    {
        return AT_PARAM_ERROR; 
    }
    b_en = enable_temp==1? true:false;
    if(app_param.lora_info.lr_ADR_en != b_en)
    {
        app_param.lora_info.lr_ADR_en = b_en;     
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+LR_ADR_EN=?\r\n-------------------------------------*/

/*------------------------AT+LR_DR_MIN=?\r\n-------------------------------------*/
ATEerror_t AT_LR_DR_MIN_get(const char *param) {
    AT_PRINTF( "%d",app_param.lora_info.lr_DR_min);       
    return AT_OK;
}

ATEerror_t AT_LR_DR_MIN_set(const char *param) {
    uint8_t DR_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &DR_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if(strlen(param) != 3)
    {
        return AT_PARAM_ERROR; 
    }
    switch(app_param.lora_info.ActiveRegion)
    {
        case LORAMAC_REGION_TTN_AS923_2:
        case LORAMAC_REGION_AS923_1:
        case LORAMAC_REGION_AS923_2:
        case LORAMAC_REGION_AS923_3:
        case LORAMAC_REGION_AS923_4:
        case LORAMAC_REGION_AS923_1B:
        case LORAMAC_REGION_TTN_AS923_1:
        case LORAMAC_REGION_AS_GP_2:
        case LORAMAC_REGION_AS_GP_3:
        case LORAMAC_REGION_AS_GP_4:
            if(DR_temp<3 || DR_temp>5)
            {
                return AT_PARAM_ERROR;                
            }
            break;
        case LORAMAC_REGION_AU915:
            if(DR_temp<3 || DR_temp>6)
            {
                return AT_PARAM_ERROR;                
            }
            break;
        case LORAMAC_REGION_US915:
            if(DR_temp<1 || DR_temp>4)
            {
                return AT_PARAM_ERROR;                
            }
            break;  
        case LORAMAC_REGION_EU868:
        case LORAMAC_REGION_KR920:
        case LORAMAC_REGION_IN865:
        case LORAMAC_REGION_RU864:
            if(DR_temp>5)
            {
                return AT_PARAM_ERROR;                
            }
            break;                                                 
        default:
            if(DR_temp>6)
            {
                return AT_PARAM_ERROR;                
            }
            break;         
    }
    if(DR_temp>app_param.lora_info.lr_DR_max)
    {
        return AT_PARAM_ERROR; 
    }
    if(app_param.lora_info.lr_DR_min != DR_temp)
    {
        app_param.lora_info.lr_DR_min = DR_temp;     
        check_save_param_type();  
    }
    return AT_OK;
}
/*------------------------AT+LR_DR_MIN=?\r\n-------------------------------------*/

/*------------------------AT+LR_DR_MAX=?\r\n-------------------------------------*/
ATEerror_t AT_LR_DR_MAX_get(const char *param) {
    AT_PRINTF( "%d",app_param.lora_info.lr_DR_max);       
    return AT_OK;
}

ATEerror_t AT_LR_DR_MAX_set(const char *param) {
    uint8_t DR_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &DR_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if(strlen(param) != 3)
    {
        return AT_PARAM_ERROR; 
    }
    switch(app_param.lora_info.ActiveRegion)
    {
        case LORAMAC_REGION_TTN_AS923_2:
        case LORAMAC_REGION_AS923_1:
        case LORAMAC_REGION_AS923_2:
        case LORAMAC_REGION_AS923_3:
        case LORAMAC_REGION_AS923_4:
        case LORAMAC_REGION_AS923_1B:
        case LORAMAC_REGION_TTN_AS923_1:
        case LORAMAC_REGION_AS_GP_2:
        case LORAMAC_REGION_AS_GP_3:
        case LORAMAC_REGION_AS_GP_4:
            if(DR_temp<3 || DR_temp>5)
            {
                return AT_PARAM_ERROR;                
            }
            break;
        case LORAMAC_REGION_AU915:
            if(DR_temp<3 || DR_temp>6)
            {
                return AT_PARAM_ERROR;                
            }
            break;
        case LORAMAC_REGION_US915:
            if(DR_temp<1 || DR_temp>4)
            {
                return AT_PARAM_ERROR;                
            }
            break;  
        case LORAMAC_REGION_EU868:
        case LORAMAC_REGION_KR920:
        case LORAMAC_REGION_IN865:
        case LORAMAC_REGION_RU864:
            if(DR_temp>5)
            {
                return AT_PARAM_ERROR;                
            }
            break;                                                  
        default:
            if(DR_temp>6)
            {
                return AT_PARAM_ERROR;                
            }
            break;         
    }
    if(DR_temp<app_param.lora_info.lr_DR_min)
    {
        return AT_PARAM_ERROR; 
    }    
    if(app_param.lora_info.lr_DR_max != DR_temp)
    {
        app_param.lora_info.lr_DR_max = DR_temp;     
        check_save_param_type(); 
    }
    return AT_OK;
}
/*------------------------AT+LR_DR_MAX=?\r\n-------------------------------------*/

/*------------------------AT+SN=?\r\n-------------------------------------*/
ATEerror_t AT_Sn_get(const char *param) {
    AT_PRINTF("\"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\"", app_param.hardware_info.Sn[0], app_param.hardware_info.Sn[1],
        app_param.hardware_info.Sn[2], app_param.hardware_info.Sn[3],
        app_param.hardware_info.Sn[4], app_param.hardware_info.Sn[5],
        app_param.hardware_info.Sn[6], app_param.hardware_info.Sn[7],
        app_param.hardware_info.Sn[8]);
    return AT_OK;
}

ATEerror_t AT_Sn_set(const char *param) {
    uint8_t sn[9];
    if ((Data_Analysis(param, sn, 9) == -1) || (strlen(param) != 28)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.hardware_info.Sn[0], sn, 9) != 0)
    {
        memcpy((uint8_t *)app_param.hardware_info.Sn, sn, 9);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+SN=?\r\n-------------------------------------*/

/*------------------------AT+DKEY=?\r\n-------------------------------------*/
ATEerror_t AT_DeviceKey_get(const char *param) {
    print_16_02x(app_param.lora_info.DeviceKey);
    return AT_OK;
}

ATEerror_t AT_DeviceKey_set(const char *param) {
    uint8_t deviceKey[16];
    if ((Data_Analysis(param, deviceKey, 16) == -1) || (strlen(param) != 49)) {
        return AT_PARAM_ERROR;
    }
    if(memcmp(&app_param.lora_info.DeviceKey[0], deviceKey, SE_KEY_SIZE) != 0)
    {
        memcpy((uint8_t *)app_param.lora_info.DeviceKey, deviceKey, SE_KEY_SIZE);
        check_save_param_type();
    }
    return AT_OK;
}
/*------------------------AT+DKEY=?\r\n-------------------------------------*/

/*------------------------AT+RETRY=?\r\n-------------------------------------*/
ATEerror_t AT_Retry_get(const char *param) {
    AT_PRINTF("%d", (int32_t)(app_param.lora_info.Retry));
    return AT_OK;
}

ATEerror_t AT_Retry_set(const char *param) {
    uint8_t retry_val = 0;
    if (tiny_sscanf(param, "%hhu", &retry_val) != 1) {
        return AT_PARAM_ERROR;
    }
    if (retry_val == RETRY_STATE_1N || retry_val == RETRY_STATE_1C) {
        if(app_param.lora_info.Retry != retry_val)
        {
            app_param.lora_info.Retry = (RetryState_t)retry_val;
            check_save_param_type();
        }
        return AT_OK;
    } else {
        return AT_PARAM_ERROR;
    }
}
/*------------------------AT+RETRY=?\r\n-------------------------------------*/

/*------------------------AT+LBDADDR=?\r\n-------------------------------------*/
ATEerror_t AT_BluetoothMacAddr_get(const char *param) {
    uint32_t ble_mac_0 = NRF_FICR->DEVICEADDR[0];
    uint32_t ble_mac_1 = NRF_FICR->DEVICEADDR[1] | 0xc000;
    AT_PRINTF("\"%02x:%02x:%02x:%02x:%02x:%02x\"", 
        (ble_mac_1 >> 8) & 0xff,
        ble_mac_1& 0xff,
        (ble_mac_0 >> 24) & 0xff,
        (ble_mac_0 >> 16) & 0xff,
        (ble_mac_0 >> 8) & 0xff,
        ble_mac_0 & 0xff);
    return AT_OK;
}
/*------------------------AT+LBDADDR=?\r\n-------------------------------------*/

/*------------------------AT+MEA=?\r\n-------------------------------------*/
/*
#define  AIR_TEMP_VAL                       "4097"  //
#define  LIGHT_LEVEL_VAL                    "4099"  //
*/
ATEerror_t AT_MeasurementValue_get(const char *param) {   
    int16_t tempture = 0;
    uint16_t lux = 0;
    int16_t ax = 0, ay = 0, az = 0;
    tempture = sensor_ntc_sample();
    lux = sensor_lux_sample();
    if( app_param.hardware_info.acc_en )
    {
        qma6100p_read_raw_data( &ax, &ay, &az );
    }
    AT_PRINTF("\r\n{\r\n\t\"4097\":\t%d",tempture*100);
    AT_PRINTF(",\r\n\t\"4199\":\t%d",lux*1000);
    if( app_param.hardware_info.acc_en )
    {
        AT_PRINTF(",\r\n\t\"4210\":\t%d",ax*1000);
        AT_PRINTF(",\r\n\t\"4211\":\t%d",ay*1000);
        AT_PRINTF(",\r\n\t\"4212\":\t%d",az*1000);
    }
    AT_PRINTF("\r\n}\r\n");
    return AT_OK;    
}
/*------------------------AT+MEA=?\r\n-------------------------------------*/

/*------------------------AT+DISCONNECT\r\n-------------------------------------*/
ATEerror_t AT_Disconnect(const char *param) {// By sending AT commands Simulate bluetooth disconnection status
    uint8_t dis_connect = 1;
    app_ble_disconnect();
    return AT_OK;
}
/*------------------------AT+DISCONNECT\r\n-------------------------------------*/

/*------------------------AT+BAT=?\r\n-------------------------------------*/
//get voltage    unit:mV
ATEerror_t AT_bat_get(const char *param) {
    uint16_t bat_volt = 0;
    bat_volt = sensor_bat_sample();
    AT_PRINTF("%d",bat_volt);
    return AT_OK;
}
/*------------------------AT+BAT=?\r\n-------------------------------------*/

/*------------------------AT+TESTMODE_TYPE=?\r\n-------------------------------------*/
ATEerror_t AT_TESTMODE_TYPE_get(const char *param) {
    AT_PRINTF("%d ",app_param.hardware_info.test_mode);     
    return AT_OK;     
}

ATEerror_t AT_TESTMODE_TYPE_set(const char *param) {
    uint8_t u8testmode = 0;
    if (strlen(param) != 3) 
    {
        return AT_PARAM_ERROR;
    }
    if (tiny_sscanf(param, "%lu", &u8testmode) != 1) 
    {
        return AT_PARAM_ERROR;
    }
    if(u8testmode != 0 && u8testmode != 1)
    {
        return AT_PARAM_ERROR;        
    }
    else
    {
        app_param.hardware_info.test_mode = u8testmode;
        check_save_param_type();
    }
    return AT_OK; 
}
/*------------------------AT+TESTMODE_TYPE=?\r\n-------------------------------------*/

/*------------------------AT+POS_STRATEGY=?\r\n-------------------------------------*/
ATEerror_t AT_POS_STRATEGY_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.pos_strategy);    
    return AT_OK;
}

ATEerror_t AT_POS_STRATEGY_set(const char *param) {
    uint8_t temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &temp))
    {
        return AT_PARAM_ERROR;        
    }
    if ((strlen(param) != 3)|| temp>7) 
    {
        return AT_PARAM_ERROR;
    }
    if(app_param.hardware_info.pos_strategy != temp)
    {
        app_param.hardware_info.pos_strategy = temp;
        check_save_param_type();             
    }
    return AT_OK;
}
/*------------------------AT+POS_STRATEGY=?\r\n-------------------------------------*/

/*------------------------AT+POS_INT=?\r\n-------------------------------------*/
ATEerror_t AT_POS_INT_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.pos_interval);  
    return AT_OK;
}

ATEerror_t AT_POS_INT_set(const char *param) {
    uint16_t interval_temp=0;
    if (1 != tiny_sscanf(param, "%hu", &interval_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if (interval_temp>7*24*60||interval_temp<1) 
    {
        return AT_PARAM_ERROR;
    }
    if(app_param.hardware_info.pos_interval != interval_temp)
    {
        app_param.hardware_info.pos_interval = interval_temp;
        check_save_param_type();
         
    }
    return AT_OK;
}
/*------------------------AT+POS_INT=?\r\n-------------------------------------*/

/*------------------------AT+SOS_MODE=?\r\n-------------------------------------*/
ATEerror_t AT_SOS_MODE_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.sos_mode); 
    return AT_OK;
}

ATEerror_t AT_SOS_MODE_set(const char *param) {
    uint8_t mode_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &mode_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if ((strlen(param) != 3)|| mode_temp>1) 
    {
        return AT_PARAM_ERROR;
    }    
    if(app_param.hardware_info.sos_mode != mode_temp)
    {
        app_param.hardware_info.sos_mode = mode_temp;  
        check_save_param_type();                     
    }
    return AT_OK;
}
/*------------------------AT+SOS_MODE=?\r\n-------------------------------------*/

/*------------------------AT+ACC_EN=?\r\n-------------------------------------*/
ATEerror_t AT_ACC_EN_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.acc_en==true?1:0); 
    return AT_OK;
}

ATEerror_t AT_ACC_EN_set(const char *param) {
    uint8_t enable_temp=0;
    bool b_en = false;
    if (1 != tiny_sscanf(param, "%hhu", &enable_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if(strlen(param) != 3 || enable_temp > 1)
    {
        return AT_PARAM_ERROR; 
    }
    b_en = enable_temp==1? true:false;
    if(app_param.hardware_info.acc_en != b_en)
    {
        app_param.hardware_info.acc_en = b_en;     
        check_save_param_type();      
    }
    return AT_OK;

}
/*------------------------AT+ACC_EN=?\r\n-------------------------------------*/

/*------------------------AT+STA_OT=?\r\n-------------------------------------*/
ATEerror_t AT_STA_OT_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.gnss_overtime);  
    return AT_OK;
}

ATEerror_t AT_STA_OT_set(const char *param) {
    uint8_t t_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &t_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if (t_temp<10 ||t_temp>120 ) 
    {
        return AT_PARAM_ERROR;
    }  
    if(app_param.hardware_info.gnss_overtime != t_temp)
    {
        app_param.hardware_info.gnss_overtime = t_temp; 
        check_save_param_type();                   
    }  
    return AT_OK;
}
/*------------------------AT+STA_OT=?\r\n-------------------------------------*/

/*------------------------AT+BEAC_OT=?\r\n-------------------------------------*/
ATEerror_t AT_BEAC_OT_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.beac_overtime);  
    return AT_OK;
}

ATEerror_t AT_BEAC_OT_set(const char *param)
{
    uint8_t t_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &t_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if (t_temp<3 ||t_temp>10 ) 
    {
        return AT_PARAM_ERROR;
    }  
    if(app_param.hardware_info.beac_overtime != t_temp)
    {
        app_param.hardware_info.beac_overtime = t_temp; 
        check_save_param_type();                 
    }
    return AT_OK;
}
/*------------------------AT+BEAC_OT=?\r\n-------------------------------------*/

/*------------------------AT+BEAC_UUID=?\r\n-------------------------------------*/
ATEerror_t AT_BEAC_UUID_get(const char *param) {
    if(app_param.hardware_info.uuid_num == 0)
    {
        AT_PRINTF("\"\"");
        return AT_OK;
    }
    AT_PRINTF("\"");
    for(uint8_t u8i = 0; u8i < app_param.hardware_info.uuid_num; u8i++)
    {
        AT_PRINTF("%02X",app_param.hardware_info.beac_uuid[u8i]);
    } 
    AT_PRINTF("\"");
    return AT_OK;    
}

ATEerror_t AT_BEAC_UUID_set(const char *param) {
    uint8_t uuid_len = 0;
    uint8_t result = 0;
    uint8_t uuid_temp[16];
    uuid_len = strlen(param)-2;
    if(uuid_len%2!=0||uuid_len>32)
    {
        return AT_PARAM_ERROR;        
    }
    for(uint8_t u8i = 0; u8i < uuid_len; u8i++)
    {
        if(isHex(param[u8i]) != 0)
        {
            return AT_PARAM_ERROR;            
        }
    } 
    for(uint8_t u8i = 0; u8i < uuid_len; u8i = u8i+2)
    {
        result = ascii_4bit_to_hex(param[u8i]);
        app_param.hardware_info.beac_uuid[u8i/2] = (result<<4)|(ascii_4bit_to_hex(param[u8i+1]));
        uuid_temp[u8i/2] = app_param.hardware_info.beac_uuid[u8i/2];
    }   
    app_param.hardware_info.uuid_num = uuid_len/2;
    check_save_param_type();
    return AT_OK;
}
/*------------------------AT+BEAC_UUID=?\r\n-------------------------------------*/

/*------------------------AT+BEAC_MAX=?\r\n-------------------------------------*/
ATEerror_t AT_BEAC_MAX_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.beac_max);  
    return AT_OK;
}

ATEerror_t AT_BEAC_MAX_set(const char *param)
{
    uint8_t t_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &t_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if (t_temp<3 || t_temp>5 ) 
    {
        return AT_PARAM_ERROR;
    }  
    if(app_param.hardware_info.beac_max != t_temp)
    {
        app_param.hardware_info.beac_max = t_temp; 
        check_save_param_type();                 
    }
    return AT_OK;
}
/*------------------------AT+BEAC_MAX=?\r\n-------------------------------------*/

/*------------------------AT+WIFI_MAX=?\r\n-------------------------------------*/
ATEerror_t AT_WIFI_MAX_get(const char *param) {
    AT_PRINTF("%d",app_param.hardware_info.wifi_max);  
    return AT_OK;
}

ATEerror_t AT_WIFI_MAX_set(const char *param)
{
    uint8_t t_temp=0;
    if (1 != tiny_sscanf(param, "%hhu", &t_temp))
    {
        return AT_PARAM_ERROR;        
    }
    if (t_temp<3 || t_temp>5 ) 
    {
        return AT_PARAM_ERROR;
    }  
    if(app_param.hardware_info.wifi_max != t_temp)
    {
        app_param.hardware_info.wifi_max = t_temp; 
        check_save_param_type();                 
    }
    return AT_OK;
}
/*------------------------AT+WIFI_MAX=?\r\n-------------------------------------*/

/*------------------------AT+POWER_OFF\r\n-------------------------------------*/
ATEerror_t AT_POWER_OFF_run(const char *param)
{
    app_toggle_power_off();
    return AT_OK;
}
/*------------------------AT+POWER_OFF\r\n-------------------------------------*/
