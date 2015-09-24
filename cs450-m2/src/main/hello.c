/* pthread.c
 * ---------
 *  Author:   <<YOUR NAME HERE>>
 *  Purpose:  This file contains the main entry point
 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us atof() */
#include <stdbool.h>            /* bool types */
#include <stdint.h>             /* uint32_t and similar types */
#include <inttypes.h>           /* PRIu** format strings */
#include <unistd.h>             /* gives us getopt() */
#include <ctype.h>              /* gives us isprint */
#include <string.h>             /* gives us strlen() */
#include <pthread.h>            /* gives us Pthreads */
#include <sys/types.h>          /* gives us gettid() */

#include "bash.h"
#include "pipe.h"

/* Local function declarations */
void show_usage (void);
bool parse_cmd (int, char *[], bool *, bool *, bool *);

/* Declare global variables here */
int global_integer = 0;

/* main entry point */
int
main(int argc, char *argv[])
{
  bool run_bash = false;
  bool run_map = false;
  bool run_pipe = false;
  bool success = parse_cmd (argc, argv, &run_bash, &run_pipe, &run_map);

  /* if we couldn't parse the command line, abort */
  if (success == false)
    {
      fprintf (stderr, "Failed to parse command line\n");
      show_usage ();
      return 1;
    }

  printf ("Hello, ");
  printf ("world\n");
  global_integer++;
  printf ("global_integer = %d\n", global_integer);

  if (run_bash)
    bash (argc, argv, optind);

  if (run_pipe)
    build_pipe ();

  if (run_map)
    sleep (100);

  return 0;
}

/* show_usage */
void
show_usage (void)
{
  printf ("Usage: pthread [options]\n");
  printf ("Options:\n");
  printf ("  -h            Print this message and exit\n");
}

/* parse_command_line */
bool
parse_cmd (int argc, char *argv[], bool *run_bash, bool *run_pipe,
           bool *run_map)
{
  int option;

  /* option is the ASCII value of the character argument passed. See
   * http://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt for
   * more documentation on using getopt(). */
  while ((option = getopt (argc, argv, "hbpa")) != -1)
    {
      switch (option)
        {
          case 'b':
            *run_bash = true;
            break;
          case 'p':
            *run_pipe = true;
            break;
          case 'a':
            *run_map = true;
            break;
          case 'h':         /* -h */
            show_usage ();
            exit (0);
            break;
          default:
            if (isprint (optopt) == true)
              {
                fprintf (stderr, "Unknown option -%c\n", optopt);
                return false;
              }
            else /* otherwise, it was an invalid flag */
              {
                fprintf (stderr, "Unknown option `\\x%x'\n", optopt);
                return false;
              }
            break;
        }
    }
  return true;
}

