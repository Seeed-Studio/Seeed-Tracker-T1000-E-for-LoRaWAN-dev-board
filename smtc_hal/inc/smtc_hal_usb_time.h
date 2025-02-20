
#ifndef __SMTC_HAL_usb_TIME__
#define __SMTC_HAL_usb_TIME__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init usb timer peripheral
 */
void hal_usb_timer_init( void );

/*!
 * @brief Deinit usb timer peripheral
 */
void hal_usb_timer_uninit( void );

#ifdef __cplusplus
}
#endif

#endif
