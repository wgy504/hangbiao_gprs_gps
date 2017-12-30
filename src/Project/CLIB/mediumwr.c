
#define NO_FLOATS
#define _FORMATTED_WRITE _medium_write
#define _CLIB_PROVIDES _Pragma("library_provides _formatted_write = \
                               qualifiers, flags, widths, n_formatters, \
                               int_specials")
#include "frmwri.c"


