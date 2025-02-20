
#ifndef __SMTC_HAL_RTC
#define __SMTC_HAL_RTC

#include <stdint.h>
#include <stdbool.h>

#define RTC_2_PER_TICK	0.091552734375
#define RTC_2_MAX_TICKS	0xffffff

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init rtc peripheral
 */
void hal_rtc_init( void );

/*!
 * @brief Get rtc time
 * 
 * @return Time in second
 */
uint32_t hal_rtc_get_time_s( void );

/*!
 * @brief Get rtc time
 * 
 * @return Time in millisecond
 */
uint32_t hal_rtc_get_time_ms( void );

/*!
 * @brief Get rtc time
 * 
 * @return Time in microsecond * 100
 */
uint32_t hal_rtc_get_time_100us( void );

/*!
 * @brief Get rtc time
 * 
 * @return Time in tick
 */
uint32_t hal_rtc_get_max_ticks( void );

/*!
 * @brief Get rtc time
 * 
 * @param [in] milliseconds timer interval
 */
void hal_rtc_wakeup_timer_set_ms( const int32_t milliseconds );

/*!
 * @brief Stop rtc wakeup timer
 */
void hal_rtc_wakeup_timer_stop( void );

/*!
 * @brief Get rtc time
 * 
 * @param [in] milliseconds timer interval
 */
void hal_rtc_cc1_timer_set_ms( const int32_t milliseconds );

/*!
 * @brief Stop rtc cc1 timer
 */
void hal_rtc_cc1_timer_stop( void );

#ifdef __cplusplus
}
#endif

#endif
