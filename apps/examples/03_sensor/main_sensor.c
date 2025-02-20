
#include "smtc_hal.h"

previous = 0;

int main(void)
{
    hal_mcu_init( );

    while( 1 )
    {
        int16_t ntc = 0, lux = 0, bat = 0;
        ntc = sensor_ntc_sample( );
        lux = sensor_lux_sample( );
        bat = sensor_bat_sample( );
        PRINTF( "ntc: %d, lux: %d, bat: %d\n", ntc, lux, bat );
        hal_mcu_wait_ms( 1000 );
    }
}
