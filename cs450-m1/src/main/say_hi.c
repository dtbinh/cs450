/* Say hi */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdint.h>             /* uint32_t and similar types */
#include <inttypes.h>           /* PRIu** format strings */
#include <unistd.h>             /* gives us getpid(), pid_t */
#include <pthread.h>            /* gives us Pthreads */

#include "pthread.h"

void *
say_hi (void * _tid)
{
  uint32_t i = 0;
  uint32_t tid = (uint32_t) _tid;

  /* Just do some busy work. */
  for (i = 0; i < 50000; i++)
  {
    if (i % 49999 == 0)
      printf ("[%u : %u : %" PRIu32 "] &i = %p; i = %" PRIu32 "\n",
              (unsigned) getpid(), (unsigned)pthread_self(), tid, &i, i);
  }
  printf ("[%u : %u : %" PRIu32 "] exiting\n", (unsigned) getpid(),
          (unsigned)pthread_self(), tid);
  pthread_exit (NULL);
}
