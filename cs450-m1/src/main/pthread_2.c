/* PTHREADS 2 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <pthread.h>            /* gives us Pthreads */
#include <assert.h>             /* gives us assert() */

#include "pthread.h"

void
pthreads_2 (void)
{
  int i = 0;
  int rc = 0;
  pthread_t threads[NUM_THREADS];

  /* PTHREADS 2 */
  for (i = 0; i < NUM_THREADS; i++)
    {
      printf ("starting thread %d\n", i);
      rc = pthread_create (&threads[i], NULL, say_hi, (void*)i);
      assert (rc == 0);
    }
  printf ("parent done\n");
  /* END PTHREADS 2 */
}
