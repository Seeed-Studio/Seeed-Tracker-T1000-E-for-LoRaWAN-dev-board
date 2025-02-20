
#ifndef __PERIPHERAL_BLE_SCAN_H__
#define __PERIPHERAL_BLE_SCAN_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sBleBeacons
{
    uint16_t company_id;
    uint8_t uuid[16];
    uint16_t major;
    uint16_t minor;
    int8_t rssi;
    int8_t rssi_;
    uint8_t mac[8];
}BleBeacons_t;

/*!
 * @brief Init ble scan
 */
void ble_scan_init( void );

/*!
 * @brief Start ble scan
 */
bool ble_scan_start( void );

/*!
 * @brief Get ble scan results
 * 
 * @param [out] result Pointer to buffer to save results
 * @param [out] size Pointer to buffer to save results length
 */
bool ble_get_results( uint8_t *result, uint8_t *size );

/*!
 * @brief Stop ble scan
 */
void ble_scan_stop( void );

/*!
 * @brief Display ble scan results
 */
void ble_display_results( void );

#ifdef __cplusplus
}
#endif

#endif
