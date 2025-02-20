

#ifndef __APP_BLE_ALL_H__
#define __APP_BLE_ALL_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "ble_scan.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t ble_rec_length;
extern uint8_t ble_rec_buff[64];
extern uint8_t is_notify_enable;
extern bool is_Communicate_with_app;

/*!
 * @brief Init ble module
 */
void app_ble_all_init( void );

/*!
 * @brief Start ble advertising
 */
void app_ble_advertising_start( void );

/*!
 * @brief Stop ble advertising
 */
void app_ble_advertising_stop( void );

/*!
 * @brief Get ble disconnected status
 * 
 * @return Disconnected status
 */
bool app_ble_is_disconnected( void );

/*!
 * @brief Disconnect ble
 */
void app_ble_disconnect( void );

/*!
 * @brief Ble data trace print
 */
void app_ble_trace_print( const char* fmt, ... );

#ifdef __cplusplus
}
#endif

#endif

