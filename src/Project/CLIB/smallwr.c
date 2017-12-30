
#define REDUCED_SUPPORT
#define _FORMATTED_WRITE _small_write
#define _CLIB_PROVIDES _Pragma("library_provides _formatted_write = qualifiers")
#include "frmwri.c"

