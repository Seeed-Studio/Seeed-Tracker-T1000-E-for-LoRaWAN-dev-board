
#ifndef __SMTC_HAL_I2C_H
#define __SMTC_HAL_I2C_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init i2c peripheral
 */
void hal_i2c_init( void );

/*!
 * @brief Deinit i2c peripheral
 */
void hal_i2c_deinit( void );

/*!
 * @brief I2c write buffer
 * 
 * @param [in] addr I2c adderss
 * @param [in] buf Pointer to buffer to be transmitted
 * @param [in] len Buffer length to be transmitted
 * @param [in] no_stop I2c stop bit
 * 
 * @return true on success, false on fail
 */
bool hal_i2c_write( uint8_t addr, uint8_t *buf, uint8_t len, bool no_stop );

/*!
 * @brief I2c read buffer
 * 
 * @param [in] addr I2c adderss
 * @param [in] buf Pointer to buffer to be received
 * @param [in] len Buffer length to be received
 * 
 * @return true on success, false on fail
 */
bool hal_i2c_read( uint8_t addr, uint8_t *buf, uint8_t len );

/*!
 * @brief I2c write buffer with register
 * 
 * @param [in] addr I2c adderss
 * @param [in] reg_addr register address
 * @param [in] buf Pointer to buffer to be transmitted
 * @param [in] len Buffer length to be transmitted
 * 
 * @return true on success, false on fail
 */
bool hal_i2c_write_reg( uint8_t addr, uint8_t reg_addr, uint8_t *buf, uint8_t len );

/*!
 * @brief I2c read buffer with register
 * 
 * @param [in] addr I2c adderss
 * @param [in] reg_addr register address
 * @param [in] buf Pointer to buffer to be received
 * @param [in] len Buffer length to be received
 * 
 * @return true on success, false on fail
 */
bool hal_i2c_read_reg( uint8_t addr, uint8_t reg_addr, uint8_t *buf, uint8_t len );

#ifdef __cplusplus
}
#endif

#endif
