
#include "app_board.h"
BoardVersion_t BoardVersion = {
    .Fields.SwMajor = 1,
    .Fields.SwMinor = 2,

    .Fields.HwMajor = 1,
    .Fields.HwMinor = 1,
};

BoardVersion_t smtc_board_version_get( void )
{
    return BoardVersion;
}
