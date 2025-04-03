
#ifndef _SMTC_HAL_GPIO_H
#define _SMTC_HAL_GPIO_H

#include <stdint.h>
#include <stdbool.h>

#define GPIO_IRQ_MAX	64

extern uint8_t btn_press_type;

typedef struct hal_gpio_irq_s
{
	uint32_t pin;
    void* context;
    void ( *callback )( void* context );
} hal_gpio_irq_t;

typedef enum gpio_state_e
{
    HAL_GPIO_RESET = 0,
    HAL_GPIO_SET   = 1,
} hal_gpio_state_t;

typedef enum gpio_pull_mode_e
{
    HAL_GPIO_PULL_MODE_NONE = 0,
    HAL_GPIO_PULL_MODE_UP   = 1,
    HAL_GPIO_PULL_MODE_DOWN = 2,
} hal_gpio_pull_mode_t;

typedef enum gpio_irq_mode_e
{
    HAL_GPIO_IRQ_MODE_OFF            = 0,
    HAL_GPIO_IRQ_MODE_RISING         = 1,
    HAL_GPIO_IRQ_MODE_FALLING        = 2,
    HAL_GPIO_IRQ_MODE_RISING_FALLING = 3,
} hal_gpio_irq_mode_t;

typedef enum gpio_pin_direction_e
{
    HAL_GPIO_PIN_DIRECTION_INPUT	= 0,
    HAL_GPIO_PIN_DIRECTION_OUTPUT	= 1,
} hal_gpio_pin_direction_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init gpio peripheral
 */
void hal_gpio_init( void );

/*!
 * @brief Init gpio peripheral
 * 
 * @param [in] pin number of pin
 * @param [in] pull_mode pull mode
 * @param [in] irq_mode interrupt mode
 * @param [in] irq interrupt callback params
 */
void hal_gpio_init_in( uint32_t pin, const hal_gpio_pull_mode_t pull_mode, const hal_gpio_irq_mode_t irq_mode, hal_gpio_irq_t* irq );

/*!
 * @brief Init gpio and output
 * 
 * @param [in] pin number of pin
 * @param [in] value output value
 */
void hal_gpio_init_out( uint32_t pin, hal_gpio_state_t value );

/*!
 * @brief Set gpio output
 * 
 * @param [in] pin number of pin
 * @param [in] value output value
 */
void hal_gpio_set_value( uint32_t pin, const hal_gpio_state_t value );

/*!
 * @brief Toggle gpio output
 * 
 * @param [in] pin number of pin
 */
void hal_gpio_toggle( uint32_t pin );

/*!
 * @brief Get gpio output
 * 
 * @param [in] pin number of pin
 * 
 * @return gpio value
 */
uint32_t hal_gpio_get_value( uint32_t pin );

/*!
 * @brief Get gpio output value
 * 
 * @param [in] pin number of pin
 * 
 * @return gpio value
 */
uint32_t hal_gpio_get_output_value( uint32_t pin );

/*!
 * @brief Get gpio and wait for state
 * 
 * @param [in] pin number of pin
 * @param [in] state the state to be wait
 */
void hal_gpio_wait_for_state( uint32_t pin, uint8_t state );

/*!
 * @brief Get gpio interrupt pending
 * 
 * @param [in] pin number of pin
 * 
 * @return interrupt pending value
 */
bool hal_gpio_is_pending_irq( uint32_t pin );

/*!
 * @brief Clear gpio interrupt pending
 * 
 * @param [in] pin number of pin
 */
void hal_gpio_clear_pending_irq( uint32_t pin );

/*!
 * @brief Attach gpio interrupt callback
 * 
 * @param [in] irq interrupt callback params
 */
void hal_gpio_irq_attach( const hal_gpio_irq_t* irq );

/*!
 * @brief Deattach gpio interrupt callback
 * 
 * @param [in] irq interrupt callback params
 */
void hal_gpio_irq_deatach( const hal_gpio_irq_t* irq );

/*!
 * @brief Enable gpio interrupt
 */
void hal_gpio_irq_enable( void );

/*!
 * @brief Disable gpio interrupt
 */
void hal_gpio_irq_disable( void );

#ifdef __cplusplus
}
#endif

#endif
