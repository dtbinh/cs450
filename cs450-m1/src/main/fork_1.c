/* FORK 1 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us exit() */
#include <unistd.h>             /* gives us fork() */
#include <inttypes.h>           /* PRIu** format strings */
#include <unistd.h>             /* gives us getpid(), pid_t */
#include <sys/wait.h>           /* gives us wait() */
#include <assert.h>             /* gives us assert() */

#include "pthread.h"

void
fork_1 (void)
{
  /* FORK exploration */
  pid_t pid = fork ();
  if (pid < 0) return;
  if (pid == 0)
    {
      printf ("[%d] pid = %d; &globvar = %p; globvar = %" PRIu32 "\n",
              getpid (), pid, &globvar, globvar);
      globvar = 5;
      printf ("[%d] pid = %d; &globvar = %p; globvar = %" PRIu32 "\n",
              getpid (), pid, &globvar, globvar);
      exit (0);
    }
  else
    {
      printf ("[%d] pid = %d; &globvar = %p; globvar = %" PRIu32 "\n",
              getpid (), pid, &globvar, globvar);
      globvar = 6;
      printf ("[%d] pid = %d; &globvar = %p; globvar = %" PRIu32 "\n",
              getpid (), pid, &globvar, globvar);
      wait (NULL);
    }
  printf ("Exiting\n");
}
