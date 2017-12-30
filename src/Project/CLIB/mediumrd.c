
#define NO_FLOATS
#define _FORMATTED_READ _medium_read
#define _CLIB_PROVIDES _Pragma("library_provides _formatted_read = \
                               assign_suppressions, scansets, n_formatters")
#include "frmrd.c"

