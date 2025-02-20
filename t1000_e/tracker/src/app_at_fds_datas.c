
#include "nrf_log.h"
#include "nrf_fstorage.h"

#ifdef SOFTDEVICE_PRESENT
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_fstorage_sd.h"
#else
#include "nrf_drv_clock.h"
#include "nrf_fstorage_nvmc.h"
#endif

#include "smtc_hal.h"
#include "app_at.h"
#include "app_at_fds_datas.h"
#include "app_config_param.h"

static bool volatile m_fds_initialized;
static fds_opt_status_t fds_opt_status = { 0 };

static const fds_access_t fds_access[1] = 
{
    { CONFIG_FILE1, CONFIG_REC_KEY1 }
};

fds_record_t m_dummy_record[1] =
{
    {   
        .file_id = CONFIG_FILE1,
        .key = CONFIG_REC_KEY1,
        .data.p_data = ( char *)( &app_param ),
        .data.length_words = ( sizeof( app_param ) + 3) / sizeof( uint32_t )
    }
};

static void fds_evt_handler( fds_evt_t const *p_evt )
{
    switch( p_evt->id ) 
    {
        case FDS_EVT_INIT:
        {
            if( p_evt->result == NRF_SUCCESS ) 
            {
                // PRINTF("FDS_EVT_INIT success\r\n");
                m_fds_initialized = true;
                fds_opt_status.fds_initial_s = true;
            }
        }
        break;

        case FDS_EVT_WRITE: 
        {
            if( p_evt->result == NRF_SUCCESS )
            {
                // PRINTF( "write----->>>Record ID:\t0x%04x\r\n", p_evt->write.record_id );
                // PRINTF( "File ID:\t0x%04x\r\n", p_evt->write.file_id);
                // PRINTF( "Record key:\t0x%04x\r\n", p_evt->write.record_key );
                // PRINTF("FDS_EVT_WRITE success\r\n");
                fds_opt_status.fds_write_s = true;
            }
        }
        break;

        case FDS_EVT_DEL_RECORD: 
        {
            if( p_evt->result == NRF_SUCCESS )
            {               
                // PRINTF( "del----->>>Record ID:\t0x%04x", p_evt->del.record_id );
                // PRINTF( "File ID:\t0x%04x", p_evt->del.file_id );
                // PRINTF("Record key:\t0x%04x\r\n", p_evt->del.record_key );
                // PRINTF( "FDS_EVT_DEL_RECORD success" );
                fds_opt_status.fds_del_record_s = true;
            }
        }
        break;
        
        case FDS_EVT_UPDATE:
        {
            if( p_evt->result == NRF_SUCCESS )
            {
                // PRINTF( "update----->>>Record ID:\t0x%04x\r\n", p_evt->del.record_id );
                // PRINTF( "File ID:\t0x%04x\r\n", p_evt->del.file_id );
                // PRINTF("Record key:\t0x%04x\r\n", p_evt->del.record_key );
                // PRINTF( "FDS_EVT_UPDATE success\r\n" );
                fds_opt_status.fds_update_s = true;
            }
        }
        break;

        case FDS_EVT_DEL_FILE:
        {
            if( p_evt->result == NRF_SUCCESS ) 
            {
                // PRINTF( "FDS_EVT_DEL_FILE success\r\n" );
                fds_opt_status.fds_del_file_s = true; 
            }  
        }
        break;

        case FDS_EVT_GC:
        {
            if( p_evt->result == NRF_SUCCESS ) 
            {
                // PRINTF( "FDS_EVT_GC success\r\n" );
                fds_opt_status.fds_gc_s = true;
            } 
        }
        break;

        default:
        break;
    }
}

void fds_init_write( void ) 
{
    ret_code_t rc;
    fds_record_desc_t desc = { 0 };
    fds_find_token_t tok = { 0 };

    (void)fds_register( fds_evt_handler ) ; // FDS callback
    rc = fds_init( );                     // fds
    APP_ERROR_CHECK( rc );

    while( !m_fds_initialized )
    {
        sd_app_evt_wait( );
    }

    waste_detect_recycle( );

    // init param
    for( uint8_t i = 0; i < 1; i ++ )
    {
        memset( &desc, 0,sizeof( desc ));
        memset( &tok, 0,sizeof( tok ));
        rc = fds_record_find( fds_access[i].config_file, fds_access[i].config_rec_key, &desc, &tok );
        if( rc == NRF_SUCCESS )
        {
            fds_flash_record_t temp = { 0 };
            rc = fds_record_open( &desc, &temp );
            APP_ERROR_CHECK( rc );
            memcpy( m_dummy_record[i].data.p_data, temp.p_data, ( temp.p_header->length_words * ( sizeof( uint32_t ))));
            NRF_LOG_INFO( "DATA:%s", temp.p_data );
            rc = fds_record_close( &desc );
            APP_ERROR_CHECK( rc );
        } 
        else // not find write a new one
        {
            // PRINTF( "fds_record_find error,errcode:%04x in fds_init_write\r\n", rc ); 
            if( !write_record_by_desc( &desc, &m_dummy_record[i] ))
            {
                PRINTF("write record error in fds_init_write\r\n");                
            }
            else // readback and check
            {
                fds_flash_record_t temp_record = { 0 };
                uint8_t rb_buff[200]={ 0 };
                uint8_t rb_length = 0;
                rc = fds_record_open( &desc, &temp_record );
                APP_ERROR_CHECK( rc );
                rb_length = temp_record.p_header->length_words * ( sizeof( uint32_t ));
                memcpy( rb_buff, temp_record.p_data, ( temp_record.p_header->length_words * ( sizeof( uint32_t ))));
                rc = fds_record_close( &desc );
                APP_ERROR_CHECK( rc );   
                if( memcmp(rb_buff,m_dummy_record[i].data.p_data,rb_length ) != 0 )
                {
                    PRINTF( "readback record error in fds_init_write\r\n" );
                }
            }
        }
    }
}

bool read_lfs_file( uint8_t file_name, uint8_t *data, uint8_t len ) 
{

    ret_code_t rc;
    fds_record_desc_t desc = { 0 };
    fds_find_token_t tok = { 0 };
    memset( &tok, 0x00, sizeof( fds_find_token_t ));
    if( file_name > DEV_INFO_FILE )
    {
        return false;
    }
    rc = fds_record_find( fds_access[file_name].config_file, fds_access[file_name].config_rec_key, &desc, &tok );
    if( rc == NRF_SUCCESS ) 
    {
        fds_flash_record_t temp = { 0 };
        rc = fds_record_open( &desc, &temp );
        APP_ERROR_CHECK( rc );
        // memcpy( data, temp.p_data, len );
        memcpy( m_dummy_record[file_name].data.p_data, temp.p_data, ( temp.p_header->length_words ) * sizeof( uint32_t ));
        NRF_LOG_INFO( "DATA:%s", temp.p_data );
        rc = fds_record_close( &desc );
        APP_ERROR_CHECK( rc );
        return true;
    }
    return false;
}

bool write_lfs_file( uint8_t file_name ) 
{
    ret_code_t rc;
    fds_record_desc_t desc = { 0 };
    fds_find_token_t tok = { 0 };
    fds_flash_record_t temp_record = { 0 };
    uint8_t rb_buff[200] = { 0 };
    uint8_t rb_length = 0;

    waste_detect_recycle( ); 
    memset( &tok, 0x00, sizeof( fds_find_token_t ));
    switch( file_name ) 
    {
        case DEV_INFO_FILE:
        {
            rc = fds_record_find( fds_access[DEV_INFO_FILE].config_file, fds_access[DEV_INFO_FILE].config_rec_key, &desc, &tok );
            if( rc == NRF_SUCCESS ) 
            {
                if( !update_record_by_desc( &desc, &m_dummy_record[DEV_INFO_FILE] ))
                {
                    PRINTF( "update DEV_INFO_FILE record error\r\n" );
                    return false; 
                }
            }
            else
            {
                PRINTF( "find DEV_INFO_FILE record error\r\n" );
                return false;
            }
        }
        break;

        default:
            return false;
        break;
    }

    rc = fds_record_open( &desc, &temp_record );
    APP_ERROR_CHECK( rc );
    rb_length = temp_record.p_header->length_words * ( sizeof( uint32_t ));
    memcpy( rb_buff, temp_record.p_data, ( temp_record.p_header->length_words * ( sizeof( uint32_t ))));
    rc = fds_record_close( &desc );
    APP_ERROR_CHECK( rc );   

    if( memcmp( rb_buff,( uint8_t *)m_dummy_record[file_name].data.p_data,rb_length ) != 0 )
    {
        PRINTF( "readback record error in write_lfs_file\r\n" );  
    }
    return true;
}

bool save_Config( void ) 
{
    if( at_config_flag & DEVICE_INFO_CHANGE )
    {
        if( !write_lfs_file( DEV_INFO_FILE )) 
        {
            at_config_flag = NO_MODIFICATION;    
            return false;
        }
    }
    at_config_flag = NO_MODIFICATION;
    return true;
}

bool read_current_param_config( void ) 
{
    if ( !read_lfs_file( DEV_INFO_FILE, ( uint8_t *)&app_param, sizeof( app_param )))
        return false;
    return true;
}

bool write_current_param_config( void )
{
    if ( !write_lfs_file( DEV_INFO_FILE ))
        return false;
    return true;
}

uint16_t record_max_length( void )
{
    uint16_t temp_length = 0;
    temp_length = m_dummy_record[0].data.length_words;
    for( uint8_t i = 1; i < 1; i++ )
    {
        if( temp_length < m_dummy_record[i].data.length_words )
        {
            temp_length = m_dummy_record[i].data.length_words;
        }
    }   
    return temp_length;
}

void waste_detect_recycle( void )
{
    static uint32_t recycle_time_count = 0;
    uint32_t temp_time = 0;
    uint16_t del_file_count = 0;
    ret_code_t rc;
    fds_stat_t stat = { 0 };
    uint16_t residual_space = 0;
    fds_record_desc_t desc = { 0 };
    fds_find_token_t tok = { 0 };

    rc = fds_stat( &stat );
    APP_ERROR_CHECK( rc );

    residual_space = stat.largest_contig;
    if( stat.largest_contig < 3 * record_max_length( ))
    {
        // recycle_time_count = hal_rtc_get_time_ms();
        if( stat.dirty_records > 0 )
        {
            fds_opt_status.fds_gc_s = false;
            rc = fds_gc( );
            APP_ERROR_CHECK( rc );
            while( true!=fds_opt_status.fds_gc_s )
            {
                sd_app_evt_wait(); //
            }
            // temp_time = hal_rtc_get_time_ms( );
            NRF_LOG_INFO( "Found %d valid records.", stat.valid_records );
            NRF_LOG_INFO( "Found %d dirty records (ready to be garbage collected).", stat.dirty_records );
        }
        
        // PRINTF( "recycle_time_count:%d  temp_time:%d\r\n",recycle_time_count,temp_time );
    }
}

bool update_record_by_desc( fds_record_desc_t * const p_desc, fds_record_t const * const p_record )
{
    ret_code_t rc;
    fds_opt_status.fds_update_s = false;
    rc = fds_record_update( p_desc, p_record );
    if( rc != NRF_SUCCESS )
    {
        PRINTF( "update_record_by_desc error,code:%0x\r\n", rc );
    }
    hal_mcu_wait_ms( 8 );
    return true == fds_opt_status.fds_update_s ? true : false;
}

bool write_record_by_desc( fds_record_desc_t * const p_desc, fds_record_t const * const p_record )
{
    ret_code_t rc;
    fds_opt_status.fds_write_s = false;
    rc = fds_record_write( p_desc, p_record );
    if( rc != NRF_SUCCESS )
    {
        PRINTF( "write_record_by_desc error,code:%0x\r\n", rc );
    }
    hal_mcu_wait_ms( 8 );
    return true == fds_opt_status.fds_write_s ? true : false;
}

void check_save_param_type(void)
{
    at_config_flag |= DEVICE_INFO_CHANGE;    
}
