
#ifndef __APP_USER_TIMER_H
#define __APP_USER_TIMER_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init user timers
 */
void app_user_timers_init( void );

/*!
 * @brief Start command parse timer
 * 
 * @param [in] cmd_type Command type
 */
void app_user_timer_parse_cmd( uint8_t cmd_type );

/*!
 * @brief User timer handler
 */
void app_user_run_process( void );

#ifdef __cplusplus
}
#endif

#endif
