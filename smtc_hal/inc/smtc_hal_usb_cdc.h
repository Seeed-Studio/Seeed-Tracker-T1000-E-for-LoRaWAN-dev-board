
#ifndef __SMTC_HAL_USB_CDC_H__
#define __SMTC_HAL_USB_CDC_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef APP_TRACKER
extern uint16_t g_usb_rec_index;
extern uint8_t g_usb_rec_buffer[256];
#endif

/*!
 * @brief Init usb cdc peripheral
 */
void hal_usb_cdc_init( void );

/*!
 * @brief Deinit usb cdc peripheral
 */
void hal_usb_cdc_deinit( void );

/*!
 * @brief Usb cdc write buffer
 * 
 * @param [in] buff Pointer to buffer to be transmitted
 * @param [in] len buffer length to be transmitted
 */
void hal_usb_cdc_write( uint8_t* buff, uint16_t len );

/*!
 * @brief Usb cdc read buffer
 * 
 * @param [in] buff Pointer to buffer to be received
 * @param [in] len buffer length to be received
 */
void hal_usb_cdc_read( uint8_t* buff, uint16_t len );

/*!
 * @brief Usb cdc event handler
 */
void hal_usb_cdc_event_queue_process( void );

/*!
 * @brief Get usb cdc connect status
 * 
 * @return Connect status
 */
bool hal_usb_cdc_is_connected( void );

#ifdef __cplusplus
}
#endif

#endif
