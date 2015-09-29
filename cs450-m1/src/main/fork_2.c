/* FORK 2 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us exit() */
#include <unistd.h>             /* gives us fork() */
#include <stdlib.h>             /* gives us atof() */
#include <inttypes.h>           /* PRIu** format strings */
#include <sys/wait.h>           /* gives us wait() */
#include <assert.h>             /* gives us assert() */

#include "pthread.h"

void
fork_2 (void)
{
  /* FORK */
  int i = 0;
  int iters = 20;
  pid_t pid = 0;

  globvar = 5000;
  printf ("[%d] globvar = %" PRIu32 "\n", getpid (), globvar);

  pid = fork ();
  if (pid < 0) return;
  if (pid == 0)
    {
      for (i = 0; i < iters; i++)
        globvar--;
      exit (0);
    }
  else
    {
      for (i = 0; i < iters; i++)
        globvar++;
      wait (NULL);
    }
  printf ("[%d] globvar = %" PRIu32 "\n", getpid (), globvar);
}
