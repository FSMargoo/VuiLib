#include <stdio.h>
#include <windows.h>

namespace vdbg
{
#ifndef _v_debug_string_length
#define _v_debug_string_length 1024
#endif

void _v_debug_printf(const char *format, _In_z_ _Printf_format_string_ char const *const parameter, ...);
void _v_debug_printf(const char *string);
} // namespace vdbg