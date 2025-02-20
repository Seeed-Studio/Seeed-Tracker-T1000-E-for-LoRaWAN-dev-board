
#include "smtc_hal.h"

int main(void)
{
    hal_mcu_init( );
    
    hal_pwm_init( 1000 );
    
    while( 1 )
    {
        hal_pwm_set_frequency( 600 );
        hal_beep_on( );
        hal_mcu_wait_ms( 500 );
        hal_beep_off( );
        hal_mcu_wait_ms( 500 );

        hal_pwm_set_frequency( 800 );
        hal_beep_on( );
        hal_mcu_wait_ms( 500 );
        hal_beep_off( );
        hal_mcu_wait_ms( 500 );

        hal_pwm_set_frequency( 1000 );
        hal_beep_on( );
        hal_mcu_wait_ms( 500 );
        hal_beep_off( );
        hal_mcu_wait_ms( 500 );
    }
}
