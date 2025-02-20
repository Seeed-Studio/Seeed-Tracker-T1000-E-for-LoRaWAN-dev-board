
#ifndef _SMTC_HAL_WATCHDOG_H
#define _SMTC_HAL_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init watch dog peripheral
 */
void hal_watchdog_init( void );

/*!
 * @brief Deinit watch dog peripheral
 */
void hal_watchdog_reload( void );

#ifdef __cplusplus
}
#endif

#endif
