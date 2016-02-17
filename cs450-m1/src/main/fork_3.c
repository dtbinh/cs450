/* FORK 3 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us exit() */
#include <unistd.h>             /* gives us fork() */
#include <sys/wait.h>           /* gives us wait() */

#include "pthread.h"

void
fork_3 (void)
{
  /* FORK 3 */
  pid_t pid = fork ();
  if (pid < 0) return;
  if (pid == 0)
    execlp ("/bin/ls", "ls", NULL);
  else
    wait (NULL);

  printf ("[%d] Exiting\n", getpid ());
}
