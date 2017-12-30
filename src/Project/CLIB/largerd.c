
#define FLOAT_SUPPORT
#define _FORMATTED_READ _large_read
#define _CLIB_PROVIDES _Pragma("library_provides _formatted_read = \
                               assign_suppressions, scansets, floats, \
                               n_formatters")
#include "frmrd.c"
