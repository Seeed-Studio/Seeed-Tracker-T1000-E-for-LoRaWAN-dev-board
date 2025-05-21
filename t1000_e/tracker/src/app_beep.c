
#include "app_timer.h"
#include "smtc_hal.h"
#include "beep_music.h"
#include "app_button.h"
#include "app_led.h"
#include "app_beep.h"

APP_TIMER_DEF(m_beep_event_timer_id);

static struct beep_song_data data;
static int song_len = 0, song_idx = 0;
static bool song_step = false;
static uint32_t song_start = 0;

uint8_t app_beep_state = APP_BEEP_IDLE;

void app_user_beep_event_timeout_handler( void )
{
    if( song_idx < song_len )
    {
        if( song_step == false )
        {
            song_step = true;
            switch( app_beep_state )
            {
                case APP_BEEP_BOOT_UP:
                {
                    beep_song_get_data( &boot_up, song_idx, &data );
                }
                break;

                case APP_BEEP_POWER_OFF:
                {
                    beep_song_get_data( &power_off, song_idx, &data );
                }
                break;

                case APP_BEEP_LORA_JOINED:
                {
                    beep_song_get_data( &joined, song_idx, &data );
                }
                break;

                case APP_BEEP_LOW_POWER:
                {
                    beep_song_get_data( &low_power, song_idx, &data );
                }
                break;

                case APP_BEEP_LORA_DOWNLINK:
                {
                    beep_song_get_data( &lora_downlink, song_idx, &data );
                }
                break;

                case APP_BEEP_SOS:
                {
                    beep_song_get_data( &sos, song_idx, &data );
                }
                break;

                case APP_BEEP_POS_S:
                {
                    beep_song_get_data( &pos_s, song_idx, &data );
                }
                break;

                default:
                break;
            }
            hal_pwm_set_frequency(data.freq);
            hal_beep_on( );
            app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
        }
        else
        {
            song_step = false;
            hal_beep_off( );
            app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.nosound_len ), NULL );
            song_idx ++;
        }
    }
    else
    {
        if( app_beep_state == APP_BEEP_SOS || app_beep_state == APP_BEEP_LORA_DOWNLINK )
        {
            if( button_sos_type == 0 && app_beep_state == APP_BEEP_SOS ) // SOS single
            {
                if(( hal_rtc_get_time_ms( ) - song_start ) > 3000 )
                {
                    app_beep_idle( );
                    return;
                }
            }

            song_idx = 0;
            song_step = true;
            switch( app_beep_state )
            {
                case APP_BEEP_SOS:
                {
                    beep_song_get_data( &sos, song_idx, &data );
                }
                break;

                case APP_BEEP_LORA_DOWNLINK:
                {
                    beep_song_get_data( &lora_downlink, song_idx, &data );
                }
                break;

                default:
                break;
            }
            hal_pwm_set_frequency(data.freq);
            hal_beep_on( );
            app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
        }
        else
        {
            app_beep_idle( );
        }
    }
}

void app_beep_boot_up( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );
    beep_song_decode_init( );
    song_len = beep_song_get_len( &boot_up );
    beep_song_get_data( &boot_up, song_idx, &data );
    hal_pwm_set_frequency(data.freq);
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_BOOT_UP;
}

void app_beep_power_off( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &power_off );
    beep_song_get_data( &power_off, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_POWER_OFF;
}

void app_beep_joined( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &joined );
    beep_song_get_data( &joined, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_LORA_JOINED;
}

void app_beep_lora_downlink( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &lora_downlink );
    beep_song_get_data( &lora_downlink, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_LORA_DOWNLINK;
}

void app_beep_low_power( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &low_power );
    beep_song_get_data( &low_power, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_LOW_POWER;
}

void app_beep_sos( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &sos );
    beep_song_get_data( &sos, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_SOS;
    song_start = hal_rtc_get_time_ms( );
}

void app_beep_pos_s( void )
{
    song_idx = 0;
    song_step = true;
    hal_pwm_init( 1000 );  
    beep_song_decode_init( );
    song_len = beep_song_get_len( &pos_s );
    beep_song_get_data( &pos_s, song_idx, &data );
    hal_pwm_set_frequency( data.freq );
    hal_beep_on( );
    app_timer_start( m_beep_event_timer_id,  APP_TIMER_TICKS( data.sound_len ), NULL );
    app_beep_state = APP_BEEP_POS_S;
}

void app_beep_idle( void )
{
    hal_beep_off( );
    hal_pwm_deinit( );
    app_timer_stop( m_beep_event_timer_id );
    app_beep_state = APP_BEEP_IDLE;
}

void app_beep_init( void )
{
    app_timer_create( &m_beep_event_timer_id, APP_TIMER_MODE_SINGLE_SHOT, app_user_beep_event_timeout_handler );
}
