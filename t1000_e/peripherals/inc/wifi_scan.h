
#ifndef __PERIPHERAL_WIFI_SCAN_H__
#define __PERIPHERAL_WIFI_SCAN_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "wifi_helpers_defs.h"

/*!
 * @brief Start wifi scan
 * 
 * @param [in] modem_radio Chip implementation context
 */
bool wifi_scan_start( ralf_t* modem_radio );

/*!
 * @brief Get wifi scan results
 * 
 * @param [in] modem_radio Chip implementation context
 * @param [out] result Pointer to buffer to save results
 * @param [out] size Pointer to buffer to save results length
 */
bool wifi_get_results( ralf_t* modem_radio, uint8_t *result, uint8_t *size );

/*!
 * @brief Stop wifi scan
 * 
 * @param [in] modem_radio Chip implementation context
 */
void wifi_scan_stop( ralf_t* modem_radio );

/*!
 * @brief Display wifi scan results
 */
void wifi_display_results( void );

#endif
