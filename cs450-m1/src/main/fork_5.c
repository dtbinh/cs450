/* FORK 5 */

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
fork_5 (void)
{
  uint32_t value = 5;

  /* FORK exploration */
  pid_t pid = fork ();
  if (pid < 0) return;
  if (pid == 0)
    {
      printf ("[%d] Value is at %p and has value %" PRIu32 "\n", getpid (),
              &value, value);
      value += 2;
      printf ("[%d] Value is at %p and has value %" PRIu32 "\n", getpid (),
              &value, value);
      globvar = 13;
      printf ("[%d] globvar is at %p and has value 0x%" PRIx32 "\n", getpid (),
              &globvar, globvar);
      exit (0);
    }
  else
    {
      wait (NULL);
    }

  printf ("===========================\n");
  printf ("[%d] globvar is at %p and has value 0x%" PRIx32 "\n", getpid (),
          &globvar, globvar);
  printf ("[%d] Value is at %p and has value %" PRIu32 "\n", getpid (), &value,
          value);
}
