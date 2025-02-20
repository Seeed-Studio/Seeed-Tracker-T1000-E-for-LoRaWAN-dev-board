
#ifndef __APP_LORA_PACKET_H
#define __APP_LORA_PACKET_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_PERSS_LONG       3000
#define BUTTON_PRESS_CLICK      500

#define BUTTON_PRESS_ONECE      1
#define BUTTON_PRESS_TWICE      2
#define BUTTON_PRESS_THRICE     3

#define APP_USER_SOS_NUM_MAX    30

extern uint8_t button_sos_type;
extern bool sos_in_progress;

/*!
 * @brief Init button module
 */
void app_user_button_init( void );

/*!
 * @brief Button detcet handler
 */
void app_user_button_det( void );

/*!
 * @brief Toggle on sod continuous
 */
void app_sos_continuous_toggle_on( void );

/*!
 * @brief Toggle off sod continuous
 */
void app_sos_continuous_toggle_off( void );

/*!
 * @brief Power off device
 */
void app_user_power_off( void );

#ifdef __cplusplus
}
#endif

#endif
