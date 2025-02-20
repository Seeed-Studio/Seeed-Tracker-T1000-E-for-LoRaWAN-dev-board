
#ifndef __SMTC_HAL_LP_TIME__
#define __SMTC_HAL_LP_TIME__

#include <stdint.h>

typedef struct hal_lp_timer_irq_s
{
    void* context;
    void ( *callback )( void* context );
} hal_lp_timer_irq_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init low-power timer 
 */
void hal_lp_timer_init( void );

/*!
 * @brief deinit low-power timer 
 */
void hal_lp_timer_deinit( void );

/*!
 * @brief Start low-power timer
 * @param [in] milliseconds timer interval
 * @param [in] tmr_irq pointer to buffer for callback params
 */
void hal_lp_timer_start( const uint32_t milliseconds, const hal_lp_timer_irq_t* tmr_irq );

/*!
 * @brief Stop low-power timer 
 */
void hal_lp_timer_stop( void );

/*!
 * @brief Enable low-power timer interrupt
 */
void hal_lp_timer_irq_enable( void );

/*!
 * @brief Disable low-power timer interrupt
 */
void hal_lp_timer_irq_disable( void );

/*!
 * @brief Low-power timer event handler
 */
void hal_lp_timer_event_handler( void );

#ifdef __cplusplus
}
#endif

#endif
