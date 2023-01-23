#define _CRT_SECURE_NO_WARNINGS

#include "../../include/debug/vdebug.h"
#include <debugapi.h>

namespace vdbg
{
void _v_debug_printf(const char *format, _In_z_ _Printf_format_string_ char const *const parameter, ...)
{
#ifdef _DEBUG
	char print_string[_v_debug_string_length];
	sprintf(print_string, format, parameter);

	OutputDebugStringA(print_string);
#else

#endif
}
void _v_debug_printf(const char *string)
{
#ifdef _DEBUG
	OutputDebugStringA(string);
#else

#endif
}
} // namespace vdbg