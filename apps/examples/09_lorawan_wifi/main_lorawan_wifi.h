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

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief Defines the delay before starting a new Wi-Fi scan, value in [s].
 */
#define WIFI_SCAN_PERIOD_DEFAULT 60

/*!
 * @brief Defines the application wifi scan time. 3s, value in [s].
 */
#define WIFI_SCAN_TIME 5

/*!
 * @brief LoRaWAN application port
 */
#define LORAWAN_APP_PORT 2

/*!
 * @brief User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE 242

/*!
 * @brief If true, then the system will not power down all peripherals when going to low power mode. This is necessary
 * to keep the LEDs active in low power mode.
 */
#define APP_PARTIAL_SLEEP true

/*
 * -----------------------------------------------------------------------------
 * --- LoRaWAN Configuration ---------------------------------------------------
 */

/*!
 * @brief LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON false

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

#ifdef __cplusplus
}
#endif

#endif  // MAIN_GEOLOCATION_WIFI_H

/*!
 * @}
 */

/* --- EOF ------------------------------------------------------------------ */
