/* PTHREADS 5 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <pthread.h>            /* gives us Pthreads */
#include <assert.h>             /* gives us assert() */
#include <inttypes.h>           /* integer format strings */

#include "pthread.h"

void * race_condition (void * _params);

void
pthreads_5 (void)
{
  /* PTHREADS 5 */
  int i = 0;
  int rc = 0;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  void *status = NULL;

  globvar = 0;

  /* Make the thread "joinable" so we can wait on it */
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  for (i = 0; i < NUM_THREADS; i++)
    {
      printf ("starting thread %d\n", i);
      rc = pthread_create (&threads[i], &attr, race_condition, (void*)i);
      assert (rc == 0);
    }

  for (i = 0; i < NUM_THREADS; i++)
    {
      rc = pthread_join (threads[i], &status);
      assert (rc == 0);
    }

  printf ("globvar = %" PRIu32 "\n", globvar);

  printf ("parent done\n");
  /* END PTHREADS 5 */
}

void *
race_condition (void * _params)
{
  uint32_t i = 0;

  for (i = 0; i < 100000; i++)
    {
      globvar++;
      globvar--;
    }

  pthread_exit (NULL);
}
