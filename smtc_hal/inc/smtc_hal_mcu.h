
#ifndef __SMTC_HAL_MCU_H
#define __SMTC_HAL_MCU_H

#include <stdbool.h>
#include <stdint.h>

#include "smtc_hal.h"

/**
 * @brief Panic function for mcu issues
 */
#ifdef __cplusplus
extern "C" {
#endif

#define TRACE_PRINTF( ... ) hal_trace_print_var( __VA_ARGS__ )

/*!
 * Panic function for mcu issues
 */
#define mcu_panic( ... )                            \
    do                                              \
    {                                               \
        TRACE_PRINTF( "mcu_panic:%s\n", __func__ ); \
        TRACE_PRINTF( "-> "__VA_ARGS__ );           \
        hal_mcu_reset( );                           \
    } while( 0 );

/*
 * -----------------------------------------------------------------------------
 * --- PUBLIC FUNCTIONS PROTOTYPES ---------------------------------------------
 */
/*!
 * @brief Init peripheral
 */
void hal_mcu_init( void );

/*!
 * @brief Disable all interrupt
 */
void hal_mcu_disable_irq( void );

/*!
 * @brief Enable all interrupt
 */
void hal_mcu_enable_irq( void );

/*!
 * @brief Init clock
 */
void hal_clock_init(void);

/*!
 * @brief Init power manage
 */
void hal_pwr_init(void);

/*!
 * @brief Software reset
 */
void hal_mcu_reset( void );

/*!
 * @brief Software delay
 * @param [in] delay time in us
 */
void hal_mcu_wait_us( const int32_t microseconds );

/*!
 * @brief Software delay
 * @param [in] delay time in ms
 */
void hal_mcu_wait_ms( const int32_t ms );

/*!
 * @brief Enable or disable partial sleep
 * @param [in] enable enable or disable
 */
void hal_mcu_partial_sleep_enable( bool enable );

/*!
 * @brief Sleep and wakeup by timer
 * @param [in] milliseconds time in ms
 */
void hal_mcu_set_sleep_for_ms( const int32_t milliseconds );

/*!
 * @brief Data format from hex to bin
 * @param [in] input Pointer to buffer to input
 * @param [out] dst Pointer to buffer to output
 * @param [in] len length of buffer
 */
void hal_hex_to_bin( char *input, uint8_t *dst, int len );

/*!
 * @brief Print data from bin to hex
 * @param [in] buf Pointer to buffer to print
 * @param [in] len length of buffer
 */
void hal_print_bin_to_hex( uint8_t *buf, uint16_t len );

/*!
 * @brief Exit sleep loop
 */
void hal_sleep_exit( void );

/*!
 * @brief Reverse copy data buffer
 * @param [out] dst Pointer to buffer to output
 * @param [in] src Pointer to buffer to input
 * @param [in] size length of buffer
 */
void memcpyr( uint8_t *dst, const uint8_t *src, uint16_t size );

#endif

#ifdef __cplusplus
}
#endif
