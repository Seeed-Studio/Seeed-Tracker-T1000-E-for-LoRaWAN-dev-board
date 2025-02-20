#ifndef MAIN_LORANWAN_SENSOR_H
#define MAIN_LORANWAN_SENSOR_H

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
 * @brief Defines the application data transmission duty cycle. 60s, value in [s].
 */
#define APP_TX_DUTYCYCLE 60

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

#endif  // MAIN_TRACKER_H

/* --- EOF ------------------------------------------------------------------ */
