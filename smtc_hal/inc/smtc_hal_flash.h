
#ifndef __SMTC_HAL_FLASH_H
#define __SMTC_HAL_FLASH_H

#define ADDR_FLASH_PAGE_SIZE (( uint32_t )0x00001000 ) // Size of Page = 4 KBytes
#define ADDR_FLASH_PAGE_0 (( uint32_t )0x00000000 ) // Base @ of Page 0, 4 KBytes */
#define ADDR_FLASH_PAGE( page ) ( ADDR_FLASH_PAGE_0 + ( page ) *ADDR_FLASH_PAGE_SIZE )
 
#define APP_FLASH_ADDR_START    0xF0000
#define APP_FLASH_ADDR_END      0xF3FFF

#define APP_FLASH_SIZE_MAX  0x4000
#define APP_FLASH_PAGE_MAX	0x04

#define ADDR_FLASH_LORAWAN_CONTEXT ADDR_FLASH_PAGE(243)
#define ADDR_FLASH_MODEM_CONTEXT ADDR_FLASH_PAGE(242)
#define ADDR_FLASH_DEVNONCE_CONTEXT ADDR_FLASH_PAGE(241)
#define ADDR_FLASH_SECURE_ELEMENT_CONTEXT ADDR_FLASH_PAGE(240)

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Init flash peripheral
 */
smtc_hal_status_t hal_flash_init( void );

/*!
 * @brief Deinit flash peripheral
 */
smtc_hal_status_t hal_flash_deinit( void );

/*!
 * @brief Erase flash data by page
 *
 * @param [in] addr Flash start address
 * @param [in] nb_page Flash erase page number
 */
smtc_hal_status_t hal_flash_erase_page( uint32_t addr, uint8_t nb_page );

/*!
 * @brief Write data to flash
 *
 * @param [in] addr Flash start address
 * @param [in] buffer Pointer to buffer to write
 * @param [in] size Buffer size to write
 */
smtc_hal_status_t hal_flash_write_buffer( uint32_t addr, const uint8_t* buffer, uint32_t size );

/*!
 * @brief Read data from flash
 *
 * @param [in] addr Flash start address
 * @param [out] buffer Pointer to buffer to read
 * @param [in] size Buffer size to read
 */
void hal_flash_read_buffer( uint32_t addr, uint8_t* buffer, uint32_t size );

#endif

#ifdef __cplusplus
}
#endif