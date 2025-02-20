#ifndef MAIN_LORAWAN_WIFI_H
#define MAIN_LORAWAN_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */
#include <stdint.h>
#include <stdbool.h>

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */
/*!
 * @brief LoRaWAN application port
 */
#define LORAWAN_APP_PORT 5

/*!
 * @brief User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE 242

/*!
 * @brief LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON false

/*!
 * @brief 
 */
#define LORWAN_SEND_INTERVAL_MIN 15

/*!
 * @brief If true, then the system will not power down all peripherals when going to low power mode. This is necessary
 * to keep the LEDs active in low power mode.
 */
#define APP_PARTIAL_SLEEP true

/*!
 * @brief Default tracker type
 */
#define TRACKER_SCAN_GNSS_ONLY      0
#define TRACKER_SCAN_WIFI_ONLY      1
#define TRACKER_SCAN_WIFI_GNSS      2
#define TRACKER_SCAN_GNSS_WIFI      3
#define TRACKER_SCAN_BLE_ONLY       4
#define TRACKER_SCAN_BLE_WIFI       5
#define TRACKER_SCAN_BLE_GNSS       6
#define TRACKER_SCAN_BLE_WIFI_GNSS  7

/*!
 * @brief Default datarate
 *
 * @remark See @ref smtc_modem_adr_profile_t
 */
#define LORAWAN_DEFAULT_DATARATE SMTC_MODEM_ADR_PROFILE_CUSTOM

#define LORAWAN_EU868_DR_MIN    0
#define LORAWAN_EU868_DR_MAX    6

#define LORAWAN_US915_DR_MIN    1
#define LORAWAN_US915_DR_MAX    4

#define LORAWAN_AU915_DR_MIN    3
#define LORAWAN_AU915_DR_MAX    6

#define LORAWAN_AS923_DR_MIN    3
#define LORAWAN_AS923_DR_MAX    6

#define LORAWAN_KR920_DR_MIN    0
#define LORAWAN_KR920_DR_MAX    5

#define LORAWAN_IN865_DR_MIN    0
#define LORAWAN_IN865_DR_MAX    5

#define LORAWAN_RU864_DR_MIN    0
#define LORAWAN_RU864_DR_MAX    6

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

bool app_send_frame( const uint8_t* buffer, const uint8_t length, bool tx_confirmed, bool emergency );

void app_tracker_new_run( uint8_t event );

void app_radio_set_sleep( void );

void app_lora_stack_suspend( void );

#ifdef __cplusplus
}
#endif

#endif  // MAIN_GEOLOCATION_WIFI_H

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
