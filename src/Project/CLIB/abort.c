/*                      - ABORT.C -

   The ANSI "abort" function.

   $Revision: 38615 $

   Copyright 1986 - 1999 IAR Systems. All rights reserved.
*/

#include "stdlib.h"

void abort(void)
{
  exit(EXIT_FAILURE);
}
