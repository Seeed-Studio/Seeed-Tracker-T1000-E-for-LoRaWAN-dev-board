
#ifndef _APP_BEEP_H_
#define _APP_BEEP_H_

#ifdef __cplusplus
extern "C" {
#endif

enum APP_BEEP_STATE_E
{
    APP_BEEP_BOOT_UP        = 0,
    APP_BEEP_POWER_OFF      = 1,
    APP_BEEP_LORA_JOINED    = 2,
    APP_BEEP_LOW_POWER      = 3,
    APP_BEEP_LORA_DOWNLINK  = 4,
    APP_BEEP_SOS            = 5,
    APP_BEEP_POS_S,
    APP_BEEP_IDLE      
};

/*!
 * @brief Init beep module
 */
void app_beep_init( void );

/*!
 * @brief Beep of boot up
 */
void app_beep_boot_up( void );

/*!
 * @brief Beep of poewr off
 */
void app_beep_power_off( void );

/*!
 * @brief Beep of lora joinded
 */
void app_beep_joined( void );

/*!
 * @brief Beep of lora downlink
 */
void app_beep_lora_downlink( void );

/*!
 * @brief Beep of low power
 */
void app_beep_low_power( void );

/*!
 * @brief Beep of sos
 */
void app_beep_sos( void );

/*!
 * @brief Beep of sos single
 */
void app_beep_pos_s( void );

/*!
 * @brief Beep of idle
 */
void app_beep_idle( void );

#ifdef __cplusplus
}
#endif

#endif
