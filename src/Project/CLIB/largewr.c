
#define FLOAT_SUPPORT
#define _FORMATTED_WRITE _large_write
#define _CLIB_PROVIDES _Pragma("library_provides _formatted_write = \
                               qualifiers, flags, widths, floats, \
                               n_formatters, int_specials")
#include "frmwri.c"

