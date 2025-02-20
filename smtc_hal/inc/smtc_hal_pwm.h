

#ifndef _SMTC_HAL_PWM_H
#define _SMTC_HAL_PWM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init pwm peripheral
 * @param [in] freq frequency in Hz
 */
void hal_pwm_init( uint32_t freq );

/*!
 * @brief Denit pwm peripheral
 */
void hal_pwm_deinit( void );

/*!
 * @brief Set pwm frequency
 * @param [in] freq frequency in Hz
 */
void hal_pwm_set_frequency( uint32_t freq );

/*!
 * @brief Turn on beep
 */
void hal_beep_on( void );

/*!
 * @brief Turn off beep
 */
void hal_beep_off( void );

#ifdef __cplusplus
}
#endif

#endif
