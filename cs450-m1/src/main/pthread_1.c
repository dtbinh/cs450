/* PTHREADS 1 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */

#include "pthread.h"

void
pthreads_1 (void)
{
  /* PTHREADS 1 */
  printf ("Starting child thread\n");
  say_hi (0);
  printf ("Back in the parent\n");
  /* END PTHREADS 1 */
}
