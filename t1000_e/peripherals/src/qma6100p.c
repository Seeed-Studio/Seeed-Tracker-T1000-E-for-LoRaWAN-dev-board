
#include "smtc_hal.h"
#include "qma6100p.h"

static void i2c_write( uint8_t reg, uint8_t var )
{
    hal_i2c_write_reg( SLAVE_ADDR, reg, (uint8_t *)&var, 1 );   
}

static uint8_t i2c_read( uint8_t reg )
{
    uint8_t data = 0;
    if( hal_i2c_read_reg( SLAVE_ADDR, reg, (uint8_t *)&data, 1 ) == true )   
    {
        return data;    
    }
    return -1;
}

void qma6100p_check( void )
{
    uint8_t chip_id = 0;      
    chip_id = i2c_read( QMA6100P_REG_CHIP_ID );
    // PRINTF ( "chip id=%d\r\n",chip_id );
    if( chip_id == 0x90 ) PRINTF( "\nQMA6100P exist\n\n" );
    else PRINTF( "\nQMA6100P not exist\n\n" );
}

void qma6100p_init( void )
{
    i2c_write( QMA6100P_REG_RESET, 0xb6 );
    nrf_delay_ms( 5 );
    i2c_write( QMA6100P_REG_RESET, 0x00 );
    nrf_delay_ms( 10 );

    qma6100p_check( );

    i2c_write( 0x11, 0x80 );
    i2c_write( 0x11, 0x84 );
    i2c_write( 0x4a, 0x20 );
    i2c_write( 0x56, 0x01 );
    i2c_write( 0x5f, 0x80 );
    nrf_delay_ms( 2 );
    i2c_write( 0x5f, 0x00 );
    nrf_delay_ms( 10 );

    i2c_write( QMA6100P_REG_RANGE, QMA6100P_RANGE_8G ); // 8G
    i2c_write( QMA6100P_REG_BW_ODR, QMA6100P_BW_100 ); // 100Hz
}

void qma6100p_read_raw_data( int16_t *ax, int16_t *ay, int16_t *az )
{
    int16_t temp = 0;
    int32_t acc_x = 0, acc_y = 0, acc_z = 0;

    temp = i2c_read( QMA6100P_REG_XOUTL ) + ( i2c_read( QMA6100P_REG_XOUTH ) << 8 );
    acc_x = temp >> 2;

    temp = i2c_read( QMA6100P_REG_YOUTL ) + ( i2c_read( QMA6100P_REG_YOUTH ) << 8 );
    acc_y = temp >> 2;

    temp = i2c_read( QMA6100P_REG_ZOUTL ) + ( i2c_read( QMA6100P_REG_ZOUTH ) << 8 );
    acc_z = temp >> 2;

    *ax = acc_x * QMA6100P_SENSITITY_8G / 1000.0;
    *ay = acc_y * QMA6100P_SENSITITY_8G / 1000.0;
    *az = acc_z * QMA6100P_SENSITITY_8G / 1000.0;
}

void qma6100p_motion_init( uint16_t any_th, uint16_t tap_th )
{
    uint32_t any_th_temp = 0;
    uint32_t tap_th_temp = 0;

    if( any_th < 10 ) any_th = 10;
    else if( any_th > 2000 ) any_th = 2000;

    if( tap_th < 10 ) tap_th = 10;
    else if( tap_th > 2000 ) tap_th = 2000;

    any_th_temp = ((( uint32_t )any_th * 10000 / 15625 ) + 5 ) / 10;
    tap_th_temp = ((( uint32_t )tap_th * 10000 / 31250 ) + 5 ) / 10;

    if( any_th_temp > 255 ) any_th_temp = 255;
    else if( any_th_temp < 1 ) any_th_temp = 1;

    if( tap_th_temp > 63 ) tap_th_temp = 63;
    else if( tap_th_temp < 1 ) tap_th_temp = 1;

    i2c_write( 0x2d, 10 ); // no motion th, Threshold=NO_MOT_TH<7:0>*16*LSB
    i2c_write( 0x2e, any_th_temp ); // any motion th,  Threshold=ANY_MOT_TH<7:0>*16*LSB, (LSB is 16.625mg, Max is 2G)
    // i2c_write( 0x2f, 0 ); // any motion high-g
    i2c_write( 0x2c, 0x8b ); // any motion dur 4*sample, no motion dur 120s
    i2c_write( 0x1a, 0x81 ); // any motion int map, no motion int map
    i2c_write( 0x18, 0xe7 ); // any motion enable, no motion enable

    // i2c_write( 0x2f, 0x00 ); // sig motion TProof 0.25s, TSkip 1.5s
    // i2c_write( 0x2f, 0x01 ); // sig motion TProof 0.25s, TSkip 1.5s
    // i2c_write( 0x19, 0x01 ); // sig motion int map 

    i2c_write( 0x16, 0x80 ); // single tap enable
    i2c_write( 0x19, 0x80 ); // single tap int map 
    i2c_write( 0x1e, 3 ); // tap quiet th 31.25*3 mg, Threshold=TAP_QUIET<5:0>*LSB(LSB is 31.25mg, Max is 2G)
    i2c_write( 0x2a, 0x07 ); // tap quiet time 20ms, shock time 75ms, durarion time 700ms
    i2c_write( 0x2b, 0xc0 | tap_th_temp ); // tap input use x/y/z, shock th 31.25 mg, Threshold=TAP_SHOCK_TH<5:0>*LSB(LSB is 31.25mg, Max is 2G)
}

uint8_t qma6100p_get_motion_status( void )
{
    uint8_t status = 0;
    status = i2c_read( 0x09 ); // get int status 0
    return status;
}

uint8_t qma6100p_get_motion_status_1( void )
{
    uint8_t status = 0;
    status = i2c_read( 0x0a ); // get int status 1
    return status;
}
