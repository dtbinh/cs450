/* PTHREADS 3 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <pthread.h>            /* gives us Pthreads */
#include <assert.h>             /* gives us assert() */

#include "pthread.h"

void
pthreads_3 (void)
{
  /* PTHREADS 3 */
  int i = 0;
  int rc = 0;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  void *status = NULL;

  /* Make the thread "joinable" so we can wait on it */
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  for (i = 0; i < NUM_THREADS; i++)
    {
      printf ("starting thread %d\n", i);
      rc = pthread_create (&threads[i], &attr, say_hi, (void*)i);
      assert (rc == 0);
    }

  for (i = 0; i < NUM_THREADS; i++)
    {
      rc = pthread_join (threads[i], &status);
      assert (rc == 0);
    }

  printf ("parent done\n");
  /* END PTHREADS 3 */
}
