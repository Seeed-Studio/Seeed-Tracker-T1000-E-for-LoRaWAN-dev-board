#ifndef __WIFI_HELPERS_H__
#define __WIFI_HELPERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include "wifi_helpers_defs.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */

/*!
 * @brief Initialise the settings for Wi-Fi scan
 *
 * @param [in] wifi_settings Wi-Fi settings \ref wifi_settings_t
 */
void smtc_wifi_settings_init( const wifi_settings_t* wifi_settings );

/*!
 * @brief Start a Wi-Fi scan
 *
 * @param [in] radio_context Chip implementation context
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_wifi_start_scan( const void* radio_context );

/*!
 * @brief Fetch the results obtained during previous Wi-Fi scan
 *
 * @param [in] radio_context Chip implementation context
 * @param [out] result Scan results \ref wifi_scan_all_result_t
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_wifi_get_results( const void* radio_context, wifi_scan_all_result_t* result );

/*!
 * @brief Tear down function for Wi-Fi scan termination actions
 *
 * This function is typically to be called when during the handling of the event of user radio access
 */
void smtc_wifi_scan_ended( void );

/*!
 * @brief Get the power consumption of the last scan
 *
 * @param [in] radio_context Chip implementation context
 * @param [out] power_consumption_uah Power consumption of the last scan in uAh
 *
 * @return a boolean: true for success, false otherwise
 */
bool smtc_wifi_get_power_consumption( const void* radio_context, uint32_t* power_consumption_uah );

#ifdef __cplusplus
}
#endif

#endif
