#ifndef __WIFI_HELPERS_DEFS_H__
#define __WIFI_HELPERS_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include "lr11xx_wifi.h"

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC MACROS -----------------------------------------------------------
 */

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC CONSTANTS --------------------------------------------------------
 */

/*!
 * @brief The maximal time to spend in preamble detection for each single scan, in ms
 */
#define WIFI_TIMEOUT_PER_SCAN_DEFAULT ( 90 )

/*!
 * @brief The time to spend scanning one channel, in ms
 */
#define WIFI_TIMEOUT_PER_CHANNEL_DEFAULT ( 300 )

/*!
 * @brief The maximal number of results to gather. Maximum value is 32
 */
#define WIFI_MAX_RESULTS ( 5 )

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC TYPES ------------------------------------------------------------
 */

/*!
 * @brief Structure representing the configuration of Wi-Fi scan
 */
typedef struct
{
    lr11xx_wifi_channel_mask_t     channels;             //!< A mask of the channels to be scanned
    lr11xx_wifi_signal_type_scan_t types;                //!< Wi-Fi types to be scanned
    uint8_t                        max_results;          //!< Maximum number of results expected for a scan
    uint32_t                       timeout_per_channel;  //!< Time to spend scanning one channel, in ms
    uint32_t timeout_per_scan;  //!< Maximal time to spend in preamble detection for each single scan, in ms
} wifi_settings_t;

/*!
 * @brief Structure representing a single scan result
 */
typedef struct
{
    lr11xx_wifi_mac_address_t        mac_address;  //!< MAC address of the Wi-Fi access point which has been detected
    lr11xx_wifi_channel_t            channel;      //!< Channel on which the access point has been detected
    lr11xx_wifi_signal_type_result_t type;         //!< Type of Wi-Fi which has been detected
    int8_t                           rssi;         //!< Strength of the detected signal
} wifi_scan_single_result_t;

/*!
 * @brief Structure representing a collection of scan results
 */
typedef struct
{
    uint8_t                   nbr_results;                //!< Number of results
    uint32_t                  power_consumption_uah;      //!< Power consumption to acquire this set of results
    uint32_t                  timestamp;                  //!< Timestamp at which the data set has been completed
    wifi_scan_single_result_t results[WIFI_MAX_RESULTS];  //!< Buffer containing the results
} wifi_scan_all_result_t;

#ifdef __cplusplus
}
#endif

#endif
