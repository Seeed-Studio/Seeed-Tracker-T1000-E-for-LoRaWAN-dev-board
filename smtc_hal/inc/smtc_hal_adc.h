
#ifndef __SMTC_HAL_ADC_H
#define __SMTC_HAL_ADC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init adc peripheral
 *
 * @param [in] channel ADC channel number
 */
void hal_adc_init( uint8_t channel );

/*!
 * @brief Uninit adc peripheral
 */
void hal_adc_uninit( void );

/*!
 * @brief Initialize adc function
 *
 * @param [out] vcc Pointer to buffer for vcc channel voltage
 * @param [out] tmp Pointer to buffer for select channel voltage
 */
void hal_adc_sample( uint16_t *vcc, uint16_t *tmp );

#ifdef __cplusplus
}
#endif

#endif
