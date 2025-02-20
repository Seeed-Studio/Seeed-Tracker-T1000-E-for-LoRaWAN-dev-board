
#ifndef __APP_AT_COMMAND_H
#define __APP_AT_COMMAND_H

#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include "app_at.h"

#ifdef __cplusplus
extern "C" {
#endif

// #define  NO_HELP

typedef enum eBleState
{
    BLE_STATE_INITIAL,
    BLE_STATE_CONNECT,
    BLE_STATE_DISCONNECT,
    BLE_STATE_CONNECT_WAIT_DISCONNECT,
    BLE_STATE_TIMEOUT,
} BleState_t;

/**
 * @brief  Structure defining an AT Command
 */
struct ATCommand_s 
{
    const char *string;                   /*< command string, after the "AT" */
    const int32_t size_string;            /*< size of the command string, not including the final \0 */
    ATEerror_t (*get)(const char *param); /*< =? after the string to get the current value*/
    ATEerror_t (*set)(const char *param); /*< = (but not =?\0) after the string to set a value */
    ATEerror_t (*run)(const char *param); /*< \0 after the string - run the command */
#if !defined(NO_HELP)
    const char *help_string; /*< to be printed when ? after the string */
#endif                       /* !NO_HELP */
};

/*!
 * @brief Parse at command
 * 
 * @param [in] cmd Pointer to buffer to be parsed
 * @param [in] length Command data length
 */
void parse_cmd(const char *cmd,uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
