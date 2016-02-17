/* bash.c */

#include <stdio.h>              /* standard I/O */
#include <stdint.h>             /* uint32_t and similar types */
#include <inttypes.h>           /* PRIu** format strings */
#include <sys/wait.h>           /* for waitpid() */
#include <stdlib.h>             /* gives us malloc() */
#include <string.h>             /* memset() and strcmp() */
#include <unistd.h>             /* fork(), getpid(), execv() */

void
bash (int argc, char ** argv, int skip)
{
  uint32_t i;
  pid_t pid;
  char ** args;
  int status;

  if (argc < skip + 2)
    {
      printf ("No arguments passed\n");
      return;
    }

  /* Implement the fork here */

  pid = fork ();

  if (pid == 0)
    {
      printf ("[%d] Child command line:\n", getpid());
      args = malloc (sizeof (char *) * (argc - skip));
      memset (args, 0, (size_t)(argc - skip));
      for (i = skip+1; i < argc; i++) args[i-(skip+1)] = argv[i];

      printf ("  %s ", argv[skip]);
      for (i = 0; i < argc - (skip+1); i++)
        printf ("%s ", args[i]);
      printf ("\n");
      execv(argv[skip], args);

      fprintf (stderr, "[%d] Child failed to start\n", getpid ());
      exit (-1);
    } else {
      printf ("[%d] Parent is waiting\n", getpid ());
      waitpid (pid, &status, 0);
    }

  printf ("[%d] Parent is done\n", getpid ());

}
