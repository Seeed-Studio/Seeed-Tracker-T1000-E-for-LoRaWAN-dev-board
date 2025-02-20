
#include "smtc_hal.h"

int main(void)
{
    hal_mcu_init( );

    gnss_init( );
    gnss_scan_start( );
    
    while( 1 )
    {
        if( gnss_get_fix_status( ))
        {
            int32_t lat = 0, lon = 0;
            gnss_get_position( &lat, &lon );
            PRINTF( "%d, %d\n", lat, lon );
        }
        else
        {
            PRINTF( "@\n" );
        }
        hal_mcu_wait_ms( 1000 );
    }
}
