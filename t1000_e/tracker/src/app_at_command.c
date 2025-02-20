
#include "app_at_command.h"
#include "app_at_fds_datas.h"

/**
 * @brief  Array corresponding to the description of each possible AT Error
 */
static const char *const ATError_description[] =
{
        "\r\nOK\r\n",                      /* AT_OK */
        "\r\nAT_ERROR\r\n",                /* AT_ERROR */
        "\r\nAT_PARAM_ERROR\r\n",          /* AT_PARAM_ERROR */
        "\r\nAT_BUSY_ERROR\r\n",           /* AT_BUSY_ERROR */
        "\r\nAT_TEST_PARAM_OVERFLOW\r\n",  /* AT_TEST_PARAM_OVERFLOW */
        "\r\nAT_NO_NETWORK_JOINED\r\n",    /* AT_NO_NET_JOINED */
        "\r\nAT_RX_ERROR\r\n",             /* AT_RX_ERROR */
        "\r\nAT_NO_CLASS_B_ENABLE\r\n",    /* AT_NO_CLASS_B_ENABLE */
        "\r\nAT_DUTYCYCLE_RESTRICTED\r\n", /* AT_DUTYCYCLE_RESTRICTED */
        "\r\nAT_CRYPTO_ERROR\r\n",         /* AT_CRYPTO_ERROR */
        "\r\nAT_SAVE_FAILED\r\n",          /* AT_SAVE_FAILED */
        "\r\nAT_READ_FAILED\r\n",          /* AT_READ_ERROR */
        "\r\nAT_DELETE_ERROR\r\n",          /* AT_READ_ERROR */        
        "\r\nerror unknown\r\n",           /* AT_MAX */
};

/**
 * @brief  Array of all supported AT Commands
 */
static const struct ATCommand_s ATCommand[] =
{
    {
        .string = AT_RESET,
        .size_string = sizeof(AT_RESET) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_RESET " Trig a MCU reset\r\n",
        #endif /* !NO_HELP */
        .get = AT_return_error,
        .set = AT_return_error,
        .run = AT_reset,
    },

    {
        .string = AT_JOINEUI,
        .size_string = sizeof(AT_JOINEUI) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_JOINEUI "=<XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>. Get or Set the App Eui\r\n",
        #endif /* !NO_HELP */
        .get = AT_JoinEUI_get,
        .set = AT_JoinEUI_set,
        .run = AT_return_error,
    },

    {
        .string = AT_NWKKEY,
        .size_string = sizeof(AT_NWKKEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT"AT_NWKKEY"=<XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>: Get or Set the Network Key\r\n",
        #endif /* !NO_HELP */
        .get = AT_NwkKey_get,
        .set = AT_NwkKey_set,
        .run = AT_return_error,
    }, 

    {
        .string = AT_APPKEY,
        .size_string = sizeof(AT_APPKEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_APPKEY "=<XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>: Get or Set the Application Key\r\n",
        #endif /* !NO_HELP */
        .get = AT_AppKey_get,
        .set = AT_AppKey_set,
        .run = AT_return_error,
    },

    {
        .string = AT_NWKSKEY,
        .size_string = sizeof(AT_NWKSKEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT"AT_NWKSKEY"=<XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>: Get or Set the Network Session Key\r\n",
        #endif /* !NO_HELP */
        .get = AT_NwkSKey_get,
        .set = AT_NwkSKey_set,
        .run = AT_return_error,
    },  

    {
        .string = AT_APPSKEY,
        .size_string = sizeof(AT_APPSKEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT"AT_APPSKEY"=<XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>: Get or Set the Application Session Key\r\n",
        #endif /* !NO_HELP */
        .get = AT_AppSKey_get,
        .set = AT_AppSKey_set,
        .run = AT_return_error,
    }, 

    {
        .string = AT_DADDR,
        .size_string = sizeof(AT_DADDR) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_DADDR "=<XX:XX:XX:XX><CR><LF>. Get or Set the Device address\r\n",
        #endif /* !NO_HELP */
        .get = AT_DevAddr_get,
        .set = AT_DevAddr_set,
        .run = AT_return_error,
    },

    {
        .string = AT_DEUI,
        .size_string = sizeof(AT_DEUI) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_DEUI "=<XX:XX:XX:XX:XX:XX:XX:XX><CR><LF>. Get or Set the Device EUI\r\n",
        #endif /* !NO_HELP */
        .get = AT_DevEUI_get,
        .set = AT_DevEUI_set,
        .run = AT_return_error,
    },

    {
        .string = AT_VER,
        .size_string = sizeof(AT_VER) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_VER "=?<CR><LF>. Get the FW version\r\n",
        #endif /* !NO_HELP */
        .get = AT_version_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },

    {
        .string = AT_BAND,
        .size_string = sizeof(AT_BAND) - 1,
        #ifndef NO_HELP
            .help_string = "AT" AT_BAND "=<BandID><CR><LF>. Get or Set the Active Region BandID=[0:AS923, 1:AU915, 2:CN470, 3:CN779, 4:EU433, 5:EU868, 6:KR920, 7:IN865, 8:US915, 9:RU864, 10:AS923_1, 11:AS923_2, 12:AS923_3, 13:AS923_4, 14:AS923_1B, 15:TTN_AS923_1]\r\n",
        #endif /* !NO_HELP */  
        .get = AT_Region_get,
        .set = AT_Region_set,
        .run = AT_return_error,
    },

    {
        .string = AT_TYPE,
        .size_string = sizeof(AT_TYPE) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_TYPE "=<ActivationType><CR><LF>. Get or Set the Active Type=[0:NONE, 1:ABP, 2:OTAA]\r\n",
        #endif /* !NO_HELP */
        .get = AT_ActivationType_get,
        .set = AT_ActivationType_set,
        .run = AT_return_error,
    },

    {
        .string = AT_CHANNEL,
        .size_string = sizeof(AT_CHANNEL) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_CHANNEL "=<channelgroup><CR><LF>. Get or Set the channel group\r\n",
        #endif /* !NO_HELP */
        .get = AT_ChannelGroup_get,
        .set = AT_ChannelGroup_set,
        .run = AT_return_error,
    },

    {
        .string = AT_CLASS,
        .size_string = sizeof(AT_CLASS) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_CLASS "=?<CR><LF>. Get the Device Class=[A, B, C]\r\n",  //=<Class><CR><LF>. Get  the Device Class=[A, B, C]\r\n
        #endif /* !NO_HELP */
        .get = AT_DeviceClass_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },

    {
        .string = AT_MEA,
        .size_string = sizeof(AT_MEA) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_MEA "=?<CR><LF>. Get the Measurement value\r\n",
        #endif /* !NO_HELP */
        .get = AT_MeasurementValue_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },

    {
        .string = AT_DISCONNECT,
        .size_string = sizeof(AT_DISCONNECT) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_DISCONNECT " If bluetooth is connection,disconnect the bluetooth connection\r\n",
        #endif /* !NO_HELP */
        .get = AT_return_error,
        .set = AT_return_error,
        .run = AT_Disconnect,
    },

    {
        .string = AT_RETEY,
        .size_string = sizeof(AT_RETEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_RETEY "=?<CR><LF>. get Retransmit data\r\n",
        #endif /* !NO_HELP */
        .get = AT_Retry_get,
        .set = AT_Retry_set,
        .run = AT_return_error,
    },

    {
        .string = AT_CONFIG,
        .size_string = sizeof(AT_CONFIG) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_CONFIG "=?<CR><LF>. Get device config\r\n",
        #endif // !NO_HELP
        .get = AT_Config_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },

    {
        .string = AT_DCODE,
        .size_string = sizeof(AT_DCODE) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_DCODE " Get or Set device code\r\n",
        #endif /* !NO_HELP */
        .get = AT_DevCODE_get,
        .set = AT_DevCODE_set,
        .run = AT_return_error,
    },

    {
        .string = AT_PLATFORM,
        .size_string = sizeof(AT_PLATFORM) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_PLATFORM " Get or Set on which platform the device is registered\r\n",
        #endif /* !NO_HELP */
        .get = AT_Platform_get,
        .set = AT_Platform_set,
        .run = AT_return_error,
    },

    {
        .string = AT_LR_ADR_EN,
        .size_string = sizeof(AT_LR_ADR_EN) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_LR_ADR_EN " Get or Set enable of LoRaWan ADR\r\n",
        #endif /* !NO_HELP */
        .get = AT_LR_ADR_EN_get,
        .set = AT_LR_ADR_EN_set,
        .run = AT_return_error,
    },

    {
        .string = AT_LR_DR_MIN,
        .size_string = sizeof(AT_LR_DR_MIN) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_LR_DR_MIN " Get or Set minimum lora data rate when disable LoRaWan ADR\r\n",
        #endif /* !NO_HELP */
        .get = AT_LR_DR_MIN_get,
        .set = AT_LR_DR_MIN_set,
        .run = AT_return_error,
    },

    {
        .string = AT_LR_DR_MAX,
        .size_string = sizeof(AT_LR_DR_MAX) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_LR_DR_MAX " Get or Set maximum lora data rate when disable LoRaWan ADR\r\n",
        #endif /* !NO_HELP */
        .get = AT_LR_DR_MAX_get,
        .set = AT_LR_DR_MAX_set,
        .run = AT_return_error,
    },

    {
        .string = AT_SN,
        .size_string = sizeof(AT_SN) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_SN " Get or Set SN number\r\n",
        #endif /* !NO_HELP */
        .get = AT_Sn_get,
        .set = AT_Sn_set,
        .run = AT_return_error,
    },

    {
        .string = AT_DKEY,
        .size_string = sizeof(AT_DKEY) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_DKEY " Get or Set device key\r\n",
        #endif /* !NO_HELP */
        .get = AT_DeviceKey_get,
        .set = AT_DeviceKey_set,
        .run = AT_return_error,
    },

    {
        .string = AT_LBDADDR,
        .size_string = sizeof(AT_LBDADDR) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_LBDADDR " Get bluetooth MAC address\r\n",
        #endif /* !NO_HELP */
        .get = AT_BluetoothMacAddr_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },

    {
        .string = AT_TESTMODE_TYPE,
        .size_string = sizeof(AT_TESTMODE_TYPE) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_TESTMODE_TYPE " Get or Set TESTMODE type\r\n",
        #endif /* !NO_HELP */
        .get = AT_TESTMODE_TYPE_get,
        .set = AT_TESTMODE_TYPE_set,
        .run = AT_return_error,
    },

    {
        .string = AT_POS_STRATEGY,
        .size_string = sizeof(AT_POS_STRATEGY) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_POS_STRATEGY " Get or Set tracker positioning strategy\r\n",
        #endif /* !NO_HELP */
        .get = AT_POS_STRATEGY_get,
        .set = AT_POS_STRATEGY_set,
        .run = AT_return_error,
    },

    {
        .string = AT_POS_INT,
        .size_string = sizeof(AT_POS_INT) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_POS_INT " Get or Set tracker positioning interval\r\n",
        #endif /* !NO_HELP */
        .get = AT_POS_INT_get,
        .set = AT_POS_INT_set,
        .run = AT_return_error,
    },

    {
        .string = AT_SOS_MODE,
        .size_string = sizeof(AT_SOS_MODE) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_SOS_MODE " Get or Set tracker SOS report mode\r\n",
        #endif /* !NO_HELP */
        .get = AT_SOS_MODE_get,
        .set = AT_SOS_MODE_set,
        .run = AT_return_error,
    },

    {
        .string = AT_ACC_EN,
        .size_string = sizeof(AT_ACC_EN) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_ACC_EN " Get or Set enable accelerometer data gathering status\r\n",
        #endif /* !NO_HELP */
        .get = AT_ACC_EN_get,
        .set = AT_ACC_EN_set,
        .run = AT_return_error,
    }, 

    {
        .string = AT_STA_OT,
        .size_string = sizeof(AT_STA_OT) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_STA_OT " Get or Set  tracker scan satellites overtime \r\n",
        #endif /* !NO_HELP */
        .get = AT_STA_OT_get,
        .set = AT_STA_OT_set,
        .run = AT_return_error,
    },

    {
        .string = AT_BEAC_OT,
        .size_string = sizeof(AT_BEAC_OT) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_BEAC_OT " Get or Set the overtime when tracker scan beacons\r\n",
        #endif /* !NO_HELP */
        .get = AT_BEAC_OT_get,
        .set = AT_BEAC_OT_set,
        .run = AT_return_error,
    },

    {
        .string = AT_BEAC_UUID,
        .size_string = sizeof(AT_BEAC_UUID) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_BEAC_UUID " Get or Set the beacon uuid for tracker to scan\r\n",
        #endif /* !NO_HELP */
        .get = AT_BEAC_UUID_get,
        .set = AT_BEAC_UUID_set,
        .run = AT_return_error,
    },

    {
        .string = AT_BEAC_MAX,
        .size_string = sizeof(AT_BEAC_MAX) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_BEAC_MAX " Get or Set the beacon number for tracker to scan\r\n",
        #endif /* !NO_HELP */
        .get = AT_BEAC_MAX_get,
        .set = AT_BEAC_MAX_set,
        .run = AT_return_error,
    },

    {
        .string = AT_WIFI_MAX,
        .size_string = sizeof(AT_WIFI_MAX) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_WIFI_MAX " Get or Set the wifi number for tracker to scan\r\n",
        #endif /* !NO_HELP */
        .get = AT_WIFI_MAX_get,
        .set = AT_WIFI_MAX_set,
        .run = AT_return_error,
    },

    {
        .string = AT_BAT,
        .size_string = sizeof(AT_BAT) - 1,
        #ifndef NO_HELP
        .help_string = "AT" AT_BAT " Get the battery Level in mV\r\n",
        #endif /* !NO_HELP */
        .get = AT_bat_get,
        .set = AT_return_error,
        .run = AT_return_error,
    },
};

/**
 * @brief  Parse a command and process it
 * @param  cmd The command
 * @param  cmd length
 */

void parse_cmd(const char *cmd, uint16_t length) 
{
    ATEerror_t status = AT_OK;
    const struct ATCommand_s *Current_ATCommand;
    int32_t i;

    if (strncmp(cmd, "AT", 2)) 
    {
        status = AT_ERROR;

    //   trace_debug("Invail cmd:%s\r\n", cmd);
    } 
    else if (cmd[2] == '\0') 
    {
        /* status = AT_OK; */
    } 
    else if (cmd[2] == '?') 
    {
        #ifdef NO_HELP
        #else
        AT_PRINTF("AT+<CMD>?        : Help on <CMD>\r\n");
        AT_PRINTF("AT+<CMD>         : Run <CMD>\r\n");
        AT_PRINTF("AT+<CMD>=<value> : Set the value\r\n");
        AT_PRINTF("AT+<CMD>=?       : Get the value\r\n");
        for (i = 0; i < (sizeof(ATCommand) / sizeof(struct ATCommand_s)); i++) 
        {
            AT_PRINTF(ATCommand[i].help_string);
        }

        #endif /* !NO_HELP */
    } 
    else 
    {
        /*excluding AT */
        if ((cmd[length - 1] != '\n') || (cmd[length - 2] != '\r')) 
        {
            status = AT_PARAM_ERROR;
        } 
        else 
        {
            status = AT_ERROR;
            cmd += 2;
            for (i = 0; i < (sizeof(ATCommand) / sizeof(struct ATCommand_s)); i++) 
            {
                if (strncmp(cmd, ATCommand[i].string, ATCommand[i].size_string) == 0) 
                {
                    Current_ATCommand = &(ATCommand[i]);
                    /* point to the string after the command to parse it */
                    cmd += Current_ATCommand->size_string;

                    /* parse after the command */
                    switch (cmd[0]) 
                    {
                        case '\r': /* nothing after the command */
                            if (cmd[1] == '\n')
                            {
                                status = Current_ATCommand->run(cmd);
                            }
                            break;
                        case '=':
                            // if ((cmd[1] == '?') && (cmd[2] == '\0'))
                            if ((cmd[1] == '?') && (cmd[2] == '\r') && (cmd[3] == '\n')) //)
                            {
                                status = Current_ATCommand->get(cmd + 1);
                            } 
                            else 
                            {
                                status = Current_ATCommand->set(cmd + 1);
                                if (status == AT_OK) 
                                {
                                    if (!save_Config()) 
                                    {
                                        status = AT_SAVE_FAILED;
                                    }
                                }
                            }
                            break;
                        case '?':
                            #ifndef NO_HELP
                            AT_PRINTF(Current_ATCommand->help_string);
                            #endif /* !NO_HELP */
                            status = AT_OK;
                            break;
                        default:
                            /* not recognized */
                            break;
                    }
                    break;
                }
            }
        }
    }
    AT_PRINTF("%s", status < AT_MAX ? ATError_description[status] : ATError_description[AT_MAX]);
}
