/* PTHREADS 4 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <pthread.h>            /* gives us Pthreads */
#include <assert.h>             /* gives us assert() */
#include <inttypes.h>           /* integer format strings */

#include "pthread.h"

uint32_t pglob = 0;

void * incr_vars (void * _params);

void
pthreads_4 (void)
{
  uint32_t plocal = 0;

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
      rc = pthread_create (&threads[i], &attr, incr_vars, (void*)&plocal);
      assert (rc == 0);
    }

  for (i = 0; i < NUM_THREADS; i++)
    {
      rc = pthread_join (threads[i], &status);
      assert (rc == 0);
    }

  printf ("pglob = %" PRIu32 "; plocal = %" PRIu32 "\n", pglob, plocal);

  printf ("parent done\n");
  /* END PTHREADS 4 */
}

void *
incr_vars (void * _params)
{
  uint32_t *pptr = (uint32_t *)_params;
  static uint32_t svar = 0;

  printf ("pptr is at %p and has value %p\n", &pptr, pptr);
  *pptr += 1;
  pglob += 1;

  svar += 1;
  printf ("svar is at %p and has value %" PRIu32 "\n", &svar, svar);

  pthread_exit (NULL);
}
