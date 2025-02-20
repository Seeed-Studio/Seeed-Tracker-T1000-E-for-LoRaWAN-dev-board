
#ifndef __PERIPHERAL_AG3335_H__
#define __PERIPHERAL_AG3335_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init gnss
 */
void gnss_init( void );

/*!
 * @brief Start gnss scan
 */
bool gnss_scan_start( void );

/*!
 * @brief Stop gnss scan
 */
void gnss_scan_stop( void );

/*!
 * @brief Get gnss fix status
 * 
 * @return Fix status
 */
bool gnss_get_fix_status( void );

/*!
 * @brief Get gnss fix status
 * 
 * @param [out] lat Pointer to buffer to be saved for latitude
 * @param [out] lon Pointer to buffer to be saved for longitude
 */
void gnss_get_position( int32_t *lat, int32_t *lon );

/*!
 * @brief Parse gnss nmea data
 * 
 * @param [in] nmea Pointer to buffer to be parsed
 */
void gnss_parse_handler( char *nmea );

#ifdef __cplusplus
}
#endif

#endif
