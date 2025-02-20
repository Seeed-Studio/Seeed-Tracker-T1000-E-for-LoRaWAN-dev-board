
#include "smtc_hal.h"

void button_irq_handler( void *obj );

hal_gpio_irq_t button_irq = {
    .pin = USER_BUTTON,
    .callback = button_irq_handler,
    .context = NULL,
};

void button_irq_handler( void *obj )
{
    PRINTF( "user button press\n" );
}

int main(void)
{
    hal_mcu_init( );

    hal_gpio_init_in( USER_BUTTON, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_RISING, &button_irq );

    while( 1 )
    {
        hal_mcu_wait_ms( 1000 );
    }
}
