
#ifndef _APP_LED_H_
#define _APP_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

enum APP_LED_STATE_E
{
    APP_LED_BLE_CFG         = 0,
    APP_LED_LORA_JOINING    = 1,
    APP_LED_LORA_JOINED,
    APP_LED_SOS,
    APP_LED_SOS_CONFIRM,
    APP_LED_LORA_DOENLINK,
    APP_LED_IDLE,
};

/*!
 * @brief Init led module
 */
void app_led_init( void );

/*!
 * @brief Led of breathe start
 */
void app_led_breathe_start( void );

/*!
 * @brief Led of breathe stop
 */
void app_led_breathe_stop( void );

/*!
 * @brief Led of ble config
 */
void app_led_ble_cfg( void );

/*!
 * @brief Led of lora joined
 */
void app_led_lora_joined( void );

/*!
 * @brief Led of sos
 */
void app_led_sos_run( void );

/*!
 * @brief Led of sos confirm
 */
void app_led_sos_confirm( void );

/*!
 * @brief Led of lora downlink
 */
void app_led_lora_downlink( void );

/*!
 * @brief Led of idle
 */
void app_led_idle( void );

/*!
 * @brief Toggle battery charge dectect
 */
void app_led_bat_new_detect( uint32_t time );

#ifdef __cplusplus
}
#endif

#endif
