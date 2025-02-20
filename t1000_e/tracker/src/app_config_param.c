
#include "app_config_param.h"

app_param_t app_param = 
{ 
    .param_version = 0,
    .lora_info = {
        .Platform = IOT_PLATFORM_SENSECAP_TTN,
        .ActiveRegion = LORAMAC_REGION_EU868,
        .ChannelGroup = 1, // sub-band form 0 - 7
        .ActivationType = ACTIVATION_TYPE_OTAA,
        .Retry = RETRY_STATE_1N,
        .lr_ADR_en = true,
        .lr_DR_min = 0,
        .lr_DR_max = 6,
        .DevEui = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .JoinEui = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .AppKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .DevAddr = 0x00000000,
        .NwkKey= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .AppSKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .NwkSKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .DeviceCode = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .DeviceKey = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    },
    .hardware_info = {
        .Sn = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
        .pos_strategy = 0, // default gnss only
        .pos_interval = 60, // default 60 min
        .sos_mode = 1, // 0: single, 1: continuous
        .acc_en = false, // true: enable, false: disable
        .gnss_overtime = 30, // default 30s
        .wifi_max = 3, // default 3
        .beac_overtime = 3, // default 3s 
        .beac_max = 3, // default 3
        .beac_uuid = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // uuid filter
        .uuid_num = 0, 
        .test_mode = 0,
    }
};

void hexTonum( unsigned char *out_data, unsigned char *in_data, unsigned short Size ) 
{
    for( unsigned char i = 0; i < Size; i++ )
    {
        out_data[2*i] = (in_data[i] >> 4);
        out_data[2*i+1] = (in_data[i] & 0x0F);  
    }
    for( unsigned char i = 0; i < 2 * Size; i++ )
    {
        if(( out_data[i] >= 0 ) && ( out_data[i] <= 9 )) 
        {
            out_data[i] = '0'+ out_data[i];
        } 
        else if(( out_data[i] >= 0x0A ) && ( out_data[i] <= 0x0F )) 
        {
            out_data[i] = 'A'- 10 + out_data[i];
        } 
        else 
        {
            return;
        }
    }
}

void numTohex( unsigned char *out_data, unsigned char *in_data, unsigned short Size ) 
{
    unsigned char temp;
    for( unsigned char i = 0; i < Size; i++ )
    {
        temp = Char2Nibble( in_data[2 * i] );
        temp = temp<<4;
        temp += Char2Nibble( in_data[2 * i + 1] );
        out_data[i] = temp;
    }
}

uint8_t Char2Nibble( char Char ) 
{
    if((( Char >= '0' ) && ( Char <= '9' ))) 
    {
        return Char - '0';
    } 
    else if((( Char >= 'a' ) && ( Char <= 'f' ))) 
    {
        return Char - 'a' + 10;
    } 
    else if(( Char >= 'A' ) && ( Char <= 'F' )) 
    {
        return Char - 'A' + 10;
    } 
    else 
    {
        return 0xF0;
    }
}
