
#include "smtc_hal.h"

void acc_irq_handler( void *obj );

hal_gpio_irq_t acc_irq = {
    .pin = ACC_INT1,
    .callback = acc_irq_handler,
    .context = NULL,
};

void acc_irq_handler( void *obj )
{
    uint8_t status_0 = qma6100p_get_motion_status( );
    uint8_t status_1 = qma6100p_get_motion_status_1( );
    if(( status_0 & 0x01 ) == 0x01 ) PRINTF( "any motion x\n" );
    if(( status_0 & 0x02 ) == 0x02 ) PRINTF( "any motion y\n" );
    if(( status_0 & 0x04 ) == 0x04 ) PRINTF( "any motion z\n" );
    if(( status_0 & 0x80 ) == 0x80 ) PRINTF( "any motion end\n" );
    if(( status_1 & 0x80 ) == 0x80 ) PRINTF( "any tap shock\n" );
}

int main(void)
{
    hal_mcu_init( );

    hal_gpio_init_out( ACC_POWER, HAL_GPIO_SET );
    hal_gpio_init_in( ACC_INT1, HAL_GPIO_PULL_MODE_NONE, HAL_GPIO_IRQ_MODE_RISING, &acc_irq );

    qma6100p_init( );
    qma6100p_motion_init( 30, 300 );

    while( 1 )
    {
        int16_t ax = 0, ay = 0, az = 0;
        qma6100p_read_raw_data( &ax, &ay, &az );
        PRINTF( "ax:%d, ay:%d, az:%d\n", ax, ay, az );
        hal_mcu_wait_ms( 1000 );
    }
}
