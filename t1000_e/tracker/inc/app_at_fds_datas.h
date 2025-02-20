
#ifndef __APP_AT_AT_FDS_DATAS_H
#define __APP_AT_AT_FDS_DATAS_H

#include <stdbool.h>
#include <stdint.h>
#include "fds.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEV_INFO_FILE       0

// device config
#define CONFIG_FILE1    ( 0x4010 )
#define CONFIG_REC_KEY1 ( 0x7010 )

// test mode
#define CONFIG_FILE2    ( 0x4050 )
#define CONFIG_REC_KEY2 ( 0x7050 )

typedef struct fds_access // access
{
    uint16_t config_file;
    uint16_t config_rec_key;
} fds_access_t;

typedef struct fds_opt_status
{
    bool fds_initial_s;
    bool fds_write_s;
    bool fds_del_record_s;
    bool fds_update_s;
    bool fds_del_file_s;     
    bool fds_gc_s;
} fds_opt_status_t;

extern fds_record_t m_dummy_record[1];

/*!
 * @brief Update record by descriptor
 * 
 * @param [in] p_desc Pointer to buffer to descriptor
 * @param [in] p_record Pointer to buffer to record
 * 
 * @return true on success, false on fail
 */
bool update_record_by_desc( fds_record_desc_t * const p_desc, fds_record_t const * const p_record );

/*!
 * @brief Write record by descriptor
 * 
 * @param [in] p_desc Pointer to buffer to descriptor
 * @param [in] p_record Pointer to buffer to record
 * 
 * @return true on success, false on fail
 */
bool write_record_by_desc( fds_record_desc_t * const p_desc, fds_record_t const * const p_record );

/*!
 * @brief Waste recycle detect
 */
void waste_detect_recycle( void );

/*!
 * @brief Write lfs data by file name
 * 
 * @param [in] file_name Index of file name 
 * 
 * @return true on success, false on fail
 */
bool write_lfs_file( uint8_t file_name );

/*!
 * @brief Read lfs data by file name
 * 
 * @param [in] file_name Index of file name 
 * @param [in] data Pointer to buffer to read
 * @param [in] len Buffer length to read
 * 
 * @return true on success, false on fail
 */
bool read_lfs_file( uint8_t file_name, uint8_t *data, uint8_t len );

/*!
 * @brief Read current config
 * 
 * @return true on success, false on fail
 */
bool read_current_param_config( void );

/*!
 * @brief Write current config
 * 
 * @return true on success, false on fail
 */
bool write_current_param_config( void );

/*!
 * @brief Get max record length
 * 
 * @return Record length
 */
uint16_t record_max_length( void );

/*!
 * @brief Init fsd and save default parmas
 */
void fds_init_write( void );

/*!
 * @brief Save config change to fds
 * 
 * @return true on success, false on fail
 */
bool save_Config( void );

/*!
 * @brief Update config change flag
 */
void check_save_param_type( void );

#ifdef __cplusplus
}
#endif

#endif