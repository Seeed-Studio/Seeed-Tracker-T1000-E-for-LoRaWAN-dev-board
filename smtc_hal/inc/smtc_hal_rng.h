
#ifndef _SMTC_HAL_RNG_H
#define _SMTC_HAL_RNG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init rng peripheral
 */
void hal_rng_init( void );

/*!
 * @brief Deinit rng peripheral
 */
void hal_rng_deinit( void );

/*!
 * @brief Get random value
 *  
 * @return random of uint32_t
 */
uint32_t hal_rng_get_random( void );

/*!
 * @brief Get random value in range
 * 
 * @param [in] val_1 range minimal
 * @param [in] val_2 range maximum
 * 
 * @return random of uint32_t
 */
uint32_t hal_rng_get_random_in_range( const uint32_t val_1, const uint32_t val_2 );

/*!
 * @brief Get random value in range
 * 
 * @param [in] val_1 range minimal
 * @param [in] val_2 range maximum
 * 
 * @return random of int32_t
 */
int32_t hal_rng_get_signed_random_in_range( const int32_t val_1, const int32_t val_2 );

#ifdef __cplusplus
}
#endif

#endif