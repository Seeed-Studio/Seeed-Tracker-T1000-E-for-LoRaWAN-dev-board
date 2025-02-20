
#ifndef __SMTC_HAL_UART__
#define __SMTC_HAL_UART__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UART0	0
#define UART1	1

#define UART_TX_RX_BUF_SIZE	256

/*!
 * @brief Init uart_0 peripheral
 */
void hal_uart_0_init( void );

/*!
 * @brief Deinit uart_0 peripheral
 */
void hal_uart_0_deinit( void );

/*!
 * @brief Uart_0 write buffer
 * 
 * @param [in] buff Pointer to buffer to be transmitted
 * @param [in] len buffer length to be transmitted
 */
void hal_uart_0_tx( uint8_t *buff, uint16_t len );

/*!
 * @brief Init uart_1 peripheral
 */
void hal_uart_1_init( void );

/*!
 * @brief Deinit uart_1 peripheral
 */
void hal_uart_1_deinit( void );

/*!
 * @brief Uart_1 flush
 */
void hal_uart_1_flush( void );

/*!
 * @brief Uart_1 read buffer
 * 
 * @param [out] p_byte Pointer to buffer to be received
 */
void hal_uart_1_get( uint8_t *p_byte );

/*!
 * @brief Uart_1 write buffer
 * 
 * @param [in] byte Byte to be transmitted
 */
void hal_uart_1_put( uint8_t byte );

/*!
 * @brief Uart_1 write buffer
 * 
 * @param [in] buff Pointer to buffer to be transmitted
 * @param [in] len Buffer length to be transmitted
 */
void hal_uart_1_tx( uint8_t* buff, uint16_t len );

/*!
 * @brief Uart_1 read buffer
 * 
 * @param [out] buff Pointer to buffer to be received
 * @param [in] len Buffer length to be received
 */
void hal_uart_1_rx( uint8_t* buff, uint16_t len );

#ifdef __cplusplus
}
#endif

#endif
