

#ifndef __APP_LORA_PACKET_H
#define __APP_LORA_PACKET_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LORAWAN_APP_DATA_MAX_SIZE 242

#define DATA_ID_UP_PACKET_POWER             0x1e
#define DATA_ID_UP_PACKET_GPS_SEN_ACC_BAT   0x1f
#define DATA_ID_UP_PACKET_WIFI_SEN_ACC_BAT  0x20
#define DATA_ID_UP_PACKET_BLE_SEN_ACC_BAT   0x21
#define DATA_ID_UP_PACKET_GPS_SEN_BAT       0x22
#define DATA_ID_UP_PACKET_WIFI_SEN_BAT      0x23
#define DATA_ID_UP_PACKET_BLE_SEN_BAT       0x24
#define DATA_ID_UP_PACKET_SEN_ACC_BAT       0x25
#define DATA_ID_UP_PACKET_SEN_BAT           0x26

#define DATA_ID_DW_PACKET_INTEVAL_PARAM     0x81
#define DATA_ID_DW_PACKET_BUZER             0x82
#define DATA_ID_DW_PACKET_TRACK_TYPE        0x86
#define DATA_ID_DW_PACKET_POWEWR_SEND       0x88
#define DATA_ID_DW_PACKET_REBOOT            0x89
#define DATA_ID_DW_PACKET_SOS_CONTINUOUS    0x8D

/*!
 * @brief Uplink power on message
 */
void app_lora_packet_power_on_uplink( void );

/*!
 * @brief Decode downlink data
 * 
 * @param [in] Pointer to buffer to be decoded
 * @param [in] Buffer length to be decoded
 */
void app_lora_packet_downlink_decode( uint8_t *buf, uint8_t len );

/*!
 * @brief Load config params
 */
void app_lora_packet_params_load( void );

#ifdef __cplusplus
}
#endif

#endif
