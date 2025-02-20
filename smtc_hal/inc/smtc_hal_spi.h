
#ifndef _SMTC_HAL_SPI_H
#define _SMTC_HAL_SPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init spi peripheral
 */
void hal_spi_init( void );

/*!
 * @brief Deinit spi peripheral
 */
void hal_spi_deinit( void );

/*!
 * @brief Spi write buffer
 * 
 * @param [in] buffer Pointer to buffer to be transmitted
 * @param [in] length buffer length to be transmitted
 */
void hal_spi_write( const uint8_t* buffer, uint16_t length );

/*!
 * @brief Spi read buffer
 * 
 * @param [out] buffer Pointer to buffer to be received
 * @param [in] length Buffer length to be received
 */
void hal_spi_read( uint8_t* buffer, uint16_t length );

/*!
 * @brief Spi write and read buffer
 * 
 * @param [in] cbuffer Pointer to buffer to be transmitted
 * @param [out] rbuffer Pointer to buffer to be received
 * @param [in] length Buffer length to be transmitted/received
 */
void hal_spi_write_read( const uint8_t* cbuffer, uint8_t* rbuffer, uint16_t length );

/*!
 * @brief Spi read buffer
 * 
 * @param [out] buffer Pointer to buffer to be received
 * @param [in] length Buffer length to be received
 * @param [in] dummy_byte The data to be transmitted
 */
void hal_spi_read_with_dummy_byte( uint8_t* buffer, uint16_t length, uint8_t dummy_byte );

/*!
 * @brief Spi write buffer
 * 
 * @param [in] id Spi id, not work on here
 * @param [in] out_data The data to be transmitted
 */
uint16_t hal_spi_in_out( const uint32_t id, const uint16_t out_data );

#ifdef __cplusplus
}
#endif

#endif
