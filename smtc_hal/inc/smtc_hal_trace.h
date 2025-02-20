
#ifndef __SMTC_HAL_TRACE_H
#define __SMTC_HAL_TRACE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Trace print api
 */
void hal_trace_print( const char* fmt, va_list argp );

/*!
 * @brief Trace print variable
 */
void hal_trace_print_var( const char* fmt, ... );

#endif

#ifdef __cplusplus
}
#endif