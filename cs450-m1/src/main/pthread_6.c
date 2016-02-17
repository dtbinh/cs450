/* PTHREADS 6 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <pthread.h>            /* gives us Pthreads */
#include <assert.h>             /* gives us assert() */
#include <inttypes.h>           /* integer format strings */
#include <stdbool.h>            /* boolean types */
#include <omp.h>                /* OpenMP */

#include "pthread.h"

void
pthreads_6 (void)
{
  /* PTHREADS 6 */
  int i = 0;

  int rc = 0;
  pthread_t threads[4];
  pthread_attr_t attr;
  void *status = NULL;

  modexp_t params[4];
  uint64_t results[4] = { 0, 0, 0, 0 };

  bool threaded = false;

  /* Set up arguments for 4 calls to modexp */
  for (i = 0; i < 4; i++)
    {
      params[i].base = base_values[i];
      params[i].power = powers[i];
      params[i].modulus = modulus[i];
    }

  if (threaded)
    {
      /* Make the thread "joinable" so we can wait on it */
      pthread_attr_init (&attr);
      pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

      /* Fix this multithreaded version to pass the arguments */
      for (i = 0; i < 4; i++)
        {
          rc = pthread_create (&threads[i], &attr, mod_exp_thread, (void *)i);
          assert (rc == 0);
        }

      for (i = 0; i < NUM_THREADS; i++)
        {
          rc = pthread_join (threads[i], &status);
          assert (rc == 0);
        }
    }
  else
    {
      for (i = 0; i < 4; i++)
        modexp (&params[i]);
        /* results[i] = modexp (&params[i]); */
    }

  /*
  for (i = 0; i < 4; i++)
    printf ("%" PRIu64 "^%" PRIu64 " mod %" PRIu64 " = %" PRIu64 "\n",
            params[i].base, params[i].power, params[i].modulus, results[i]);
            */

  /* END PTHREADS 6 */
}

void *
mod_exp_thread (void * _args)
{
  printf ("Running modular exponentiation\n");
  pthread_exit (NULL);
}

void
modexp (modexp_t *args)
{
  modular_exponentiation (args->base, args->power, args->modulus);

  /* Later:
  return modular_exponentiation (args->base, args->power, args->modulus);
   */
}

void
modular_exponentiation (uint64_t base, uint64_t power, uint64_t mod)
{
  uint64_t ctr = 0;
  uint64_t value = base;

  for (ctr = 1; ctr < power; ctr++)
  {
    value *= base;
    value %= mod;
  }
  printf ("%" PRIu64 "^%" PRIu64 " mod %" PRIu64 " = %" PRIu64 "\n",
          base, power, mod, value);
}
