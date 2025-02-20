
#ifndef _PERIPHERAL_SENSOR_H_
#define _PERIPHERAL_SENSOR_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Get temperature value
 * 
 * @return temperature value, in celsius
 */
int16_t sensor_ntc_sample( void );

/*!
 * @brief Get light value
 * 
 * @return light value, in percentage
 */
int16_t sensor_lux_sample( void );

/*!
 * @brief Get battery value
 * 
 * @return battery value, in percentage
 */
int16_t sensor_bat_sample( void );

#endif

#ifdef __cplusplus
}
#endif
