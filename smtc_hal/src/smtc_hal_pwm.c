
#include "smtc_hal.h"
#include "app_pwm.h"

APP_PWM_INSTANCE( PWM1, 1 );        // Create the instance "PWM1" using TIMER1.

static volatile bool ready_flag;    // A flag indicating PWM status.

static bool pwm_init = false;
static bool pwm_status = false;

void pwm_ready_callback( uint32_t pwm_id )    // PWM callback function
{
    ready_flag = true;
}

void hal_pwm_init( uint32_t freq )
{
    if( freq > 10000 || freq == 0 ) return;

    if( pwm_init == false )
    {
        pwm_init = true;

        ret_code_t err_code;
        uint32_t period_us = 1000000 / freq;

        app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH( period_us, BUZZER_PWM );

        /* Switch the polarity of the second channel. */
        pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;

        /* Initialize and enable PWM. */
        err_code = app_pwm_init( &PWM1, &pwm1_cfg, pwm_ready_callback );
        APP_ERROR_CHECK( err_code );
    }
}

void hal_pwm_deinit( void )
{
    if( pwm_init == true )
    {
        pwm_init = false;
        app_pwm_disable( &PWM1 );
        app_pwm_uninit( &PWM1 );
    }
}

void hal_pwm_set_frequency( uint32_t freq )
{
    if( freq > 10000 || freq == 0 ) return;

    ret_code_t err_code;
    uint32_t period_us = 1000000 / freq;

    if( pwm_init == true )
    {
        app_pwm_disable( &PWM1 );   // disable PWM for change PWM
        app_pwm_uninit( &PWM1 );    // uninitializing a PWM
        
        app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH( period_us, BUZZER_PWM );

        /* Switch the polarity of the second channel. */
        pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

        /* Initialize and enable PWM. */
        err_code = app_pwm_init( &PWM1, &pwm1_cfg, pwm_ready_callback );
        APP_ERROR_CHECK( err_code );
    }
}

void hal_beep_on( void )
{
    if( pwm_status == false )
    {
        pwm_status = true;
        hal_gpio_init_out( BUZZER_POWER_EN, HAL_GPIO_SET );
        app_pwm_enable( &PWM1 ); // 
        app_pwm_channel_duty_set( &PWM1, 0, 50 );
    }
}

void hal_beep_off( void )
{
    if( pwm_status == true )
    {
        pwm_status = false;
        app_pwm_disable( &PWM1 );
        hal_gpio_init_out( BUZZER_POWER_EN, HAL_GPIO_RESET );
    }
}
