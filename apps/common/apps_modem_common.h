#ifndef APPS_MODEM_COMMON_H
#define APPS_MODEM_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * -----------------------------------------------------------------------------
 * --- DEPENDENCIES ------------------------------------------------------------
 */

#include <stdint.h>

#include "smtc_modem_api.h"

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
 * @brief Configure LoRaWAN parameters (DevEUI, JoinEUI, AppKey, region and class)
 *
 * @remark All parameters are defined in lorawan_key_config.h file
 *
 * @param [in] stack_id Stack identifier
 */
void apps_modem_common_configure_lorawan_params( uint8_t stack_id );

/*!
 * @brief Display the Lora Basics Modem current version
 */
void apps_modem_common_display_lbm_version( void );

#ifdef __cplusplus
}
#endif

#endif
