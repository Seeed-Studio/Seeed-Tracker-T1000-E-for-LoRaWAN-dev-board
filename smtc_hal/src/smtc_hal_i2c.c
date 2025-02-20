
#include "nrf_drv_twi.h"
#include "smtc_hal.h"

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE( 0 );
static const nrf_drv_twi_config_t twi_config = {
    .scl                = ACC_I2C_SCL,
    .sda                = ACC_I2C_SDA,
    .frequency          = NRF_DRV_TWI_FREQ_100K,
    .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
    .clear_bus_init     = false
};

static bool i2c_init = false; 

void hal_i2c_init( void )
{
    ret_code_t err_code;
    if( i2c_init == false )
    {
        i2c_init = true;
        err_code = nrf_drv_twi_init( &m_twi, &twi_config, NULL, NULL );
        APP_ERROR_CHECK( err_code );
        nrf_drv_twi_enable( &m_twi );
    }
}

void hal_i2c_deinit( void )
{
    if( i2c_init == true )
    {
        i2c_init = false;
        nrf_drv_twi_disable(&m_twi); 
        nrf_drv_twi_uninit( &m_twi );
    }
}

bool hal_i2c_write( uint8_t addr, uint8_t *buf, uint8_t len, bool no_stop )
{
    uint16_t errCode = nrf_drv_twi_tx( &m_twi, addr, buf, len, no_stop );
    if( errCode == NRF_SUCCESS ) return true;
    return false;
}

bool hal_i2c_read( uint8_t addr, uint8_t *buf, uint8_t len )
{
    uint16_t errCode = nrf_drv_twi_rx( &m_twi, addr, buf, len );
    if( errCode == NRF_SUCCESS ) return true;
    return false;
}

bool hal_i2c_write_reg( uint8_t addr, uint8_t reg_addr, uint8_t *buf, uint8_t len )
{
    uint8_t buffer[3] = { 0 };

    buffer[0] = reg_addr;
    for( uint8_t i = 0; i < len; i++ )
    {
        buffer[1 + i] = buf[i];
    }
    
    if( hal_i2c_write( addr, buffer, len + 1, false ) != true )
    {
        return false;
    }
    return true;
}

bool hal_i2c_read_reg( uint8_t addr, uint8_t reg_addr, uint8_t *buf, uint8_t len )
{
    uint8_t reg = reg_addr;
    if( hal_i2c_write( addr, &reg, 1, true ) != true )
    {
        return false;
    }

    if( hal_i2c_read( addr, buf, len ) != true )
    {
        return false;
    }

    return true;
}
