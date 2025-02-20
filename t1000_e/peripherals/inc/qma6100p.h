
#ifndef __PERIPHERAL_QMA6100P_H__
#define __PERIPHERAL_QMA6100P_H__

#include "qma6100p_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Detect qma6100p chip id
 */
void qma6100p_check( void );

/*!
 * @brief Init qma6100p
 */
void qma6100p_init( void );

/*!
 * @brief Get accelerometer raw data
 * 
 * @param [out] ax Pointer to buffer to save axle-x value
 * @param [out] ay Pointer to buffer to save axle-y value
 * @param [out] az Pointer to buffer to save axle-z value
 */
void qma6100p_read_raw_data( int16_t *ax, int16_t *ay, int16_t *az );

/*!
 * @brief Init qma6100p motion params
 * 
 * @param [in] any_th Threshold for motion event
 * @param [in] tap_th Threshold for tap event
 */
void qma6100p_motion_init( uint16_t any_th, uint16_t tap_th );

/*!
 * @brief Get qma6100p motion status
 * 
 * @return motion register status 0
 */
uint8_t qma6100p_get_motion_status( void );

/*!
 * @brief Get qma6100p motion status
 * 
 * @return motion register status 1
 */
uint8_t qma6100p_get_motion_status_1( void );

#ifdef __cplusplus
}
#endif

#endif
