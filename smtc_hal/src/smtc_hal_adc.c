
#include "smtc_hal.h"
#include "nrf_delay.h"
#include "nrf_drv_saadc.h"

#define ADC_SAMPLE_NUM_MAX  16
#define VCC_ADC_CHANNEL     NRF_SAADC_INPUT_VDD

static nrf_saadc_value_t m_buffer_pool[2];
static float adc_value[2];

static void saadc_callback( nrf_drv_saadc_evt_t const * p_event )
{
    if( p_event->type == NRF_DRV_SAADC_EVT_DONE )
    {
        m_buffer_pool[0] = p_event->data.done.p_buffer[0];
        m_buffer_pool[1] = p_event->data.done.p_buffer[1];

        adc_value[0] = (float) m_buffer_pool[0] * 3000 / 4096;
        adc_value[1] = (float) m_buffer_pool[1] * 3000 / 4096;

        nrf_drv_saadc_buffer_convert( m_buffer_pool, 2 );
    }
    else if( p_event->type == NRF_DRV_SAADC_EVT_CALIBRATEDONE )
    {
        m_buffer_pool[0] = 0;
        m_buffer_pool[1] = 0;
        nrf_drv_saadc_buffer_convert( m_buffer_pool, 2 );
    }
}

void hal_adc_init( uint8_t channel )
{
    nrf_drv_saadc_config_t adc_config = NRF_DRV_SAADC_DEFAULT_CONFIG;
    adc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
    adc_config.oversample = NRF_SAADC_OVERSAMPLE_DISABLED;
    nrf_drv_saadc_init( &adc_config, saadc_callback );

    nrf_saadc_channel_config_t channel_config_0 = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE( VCC_ADC_CHANNEL );
    nrf_saadc_channel_config_t channel_config_1 = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE( channel );
    
    channel_config_0.gain = NRF_SAADC_GAIN1_5; // max input 3000mV
    channel_config_0.acq_time = NRF_SAADC_ACQTIME_40US;
    channel_config_0.reference = NRF_SAADC_REFERENCE_INTERNAL;

    channel_config_1.gain = NRF_SAADC_GAIN1_5; // max input 3000mV
    channel_config_1.acq_time = NRF_SAADC_ACQTIME_40US;
    channel_config_1.reference = NRF_SAADC_REFERENCE_INTERNAL;

    nrf_drv_saadc_channel_init( 0, &channel_config_0 );
    nrf_drv_saadc_channel_init( 1, &channel_config_1 );
    nrf_drv_saadc_buffer_convert( m_buffer_pool, 2 );

    nrf_drv_saadc_abort( );
    nrf_drv_saadc_calibrate_offset( );
    nrf_delay_us( 2000 );
}

void hal_adc_uninit( void )
{
    __WFE(); __SEV(); __WFE(); // TODO, need to confirm this???
    nrf_drv_saadc_uninit( );
}

void hal_adc_sample( uint16_t *vcc, uint16_t *tmp )
{
    uint16_t sample_vcc_volt[ADC_SAMPLE_NUM_MAX] = { 0 };
    uint16_t sample_tmp_volt[ADC_SAMPLE_NUM_MAX] = { 0 };
    uint16_t temp_vcc_volt = 0;
    uint16_t temp_tmp_volt = 0;
    float sum_vcc_volt = 0, avg_vcc_volt = 0;
    float sum_tmp_volt = 0, avg_tmp_volt = 0;
    uint8_t sample_vcc_cnt = 0, sample_tmp_cnt = 0;

    for( uint8_t i = 0; i < ADC_SAMPLE_NUM_MAX; i ++ )
    {
        nrf_drv_saadc_sample( );
        nrf_delay_us( 500 );
        
        sample_vcc_volt[i] = adc_value[0] + 0.5;
        sample_tmp_volt[i] = adc_value[1] + 0.5;
        adc_value[0] = 0;
        adc_value[1] = 0;
    }

    for( uint8_t i = 0; i < ADC_SAMPLE_NUM_MAX; i ++ )
    {
        for( uint8_t j = i + 1; j < ADC_SAMPLE_NUM_MAX; j ++ )  
        {
            if( sample_vcc_volt[i] > sample_vcc_volt[j] )
            {
                temp_vcc_volt = sample_vcc_volt[j];
                sample_vcc_volt[i] = temp_vcc_volt;
                sample_vcc_volt[j] = sample_vcc_volt[i];
            }

            if( sample_tmp_volt[i] > sample_tmp_volt[j] )
            {
                temp_tmp_volt = sample_tmp_volt[j];
                sample_tmp_volt[i] = temp_tmp_volt;
                sample_tmp_volt[j] = sample_tmp_volt[i];
            }
        }        
    }

    for( uint8_t i = 0; i < ADC_SAMPLE_NUM_MAX; i ++ )
    {
        if( sample_vcc_volt[i] )
        {
            sample_vcc_cnt ++;
            sum_vcc_volt += sample_vcc_volt[i];
        }

        if( sample_tmp_volt[i] )
        {
            sample_tmp_cnt ++;
            sum_tmp_volt += sample_tmp_volt[i];
        }
    }

    avg_vcc_volt = ( sum_vcc_volt / ( sample_vcc_cnt > 0 ? sample_vcc_cnt : 1 )) + 0.5;
    avg_tmp_volt = ( sum_tmp_volt / ( sample_tmp_cnt > 0 ? sample_tmp_cnt : 1 )) + 0.5;
    *vcc = avg_vcc_volt;
    *tmp = avg_tmp_volt;
}
