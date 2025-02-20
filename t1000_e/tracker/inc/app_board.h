
#ifndef __APP_BOARD_H
#define __APP_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef union BoardVersion_u
{
    struct BoardVersion_s
    {
        uint8_t SwMinor;
        uint8_t SwMajor;
        uint8_t HwMinor;
        uint8_t HwMajor;
    }Fields;
    uint8_t Value[4];
}BoardVersion_t;

/*!
 * @brief get borad version
 */
BoardVersion_t smtc_board_version_get( void );

#ifdef __cplusplus
}
#endif

#endif
