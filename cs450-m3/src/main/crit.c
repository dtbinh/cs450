/* crit.c
 * ------
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

#include "../synch/synch.h"
#include "../lib/list.h"
#include "say_hi.h"
#include "chat.h"
#include "sum_all.h"
#include "incr.h"

/* Local function declarations */
void show_usage (void);
bool parse_cmd (int, char *[], bool *);

/* Declare global variables here */

/* main entry point */
int
main(int argc, char *argv[])
{
  time_t tm;
  bool chat = false;
  bool success = parse_cmd (argc, argv, &chat);

  /* if we couldn't parse the command line, abort */
  if (success == false)
    {
      fprintf (stderr, "Failed to parse command line\n");
      show_usage ();
      return 1;
    }

  /* Prepare a pseudo-random number generator */
  srand ((unsigned) time (&tm));

  if (chat)
    start_chat ();
  else
    lock_and_say_hi ();

  /* Other code: */
  //sum_all ();
  //incr_all ();

  return 0;
}

/* show_usage
 * ----------
 *  Arguments: none
 *  Returns:   none
 *  Purpose:   prints out the help usage message
 */

void
show_usage (void)
{
  printf ("Usage: pthread [options]\n");
  printf ("Options:\n");
  printf ("  -h            Print this message and exit\n");
}

/* parse_command_line
 * ------------------
 *  Arguments: int argc - number of command-line arguments
 *             char * argv[] - array of argument strings
 *             char ** output - name of output file to produce
 *             double * xstart - x value of lower-left corner (by reference)
 *             double * ystart - y value of lower-left corner (by reference)
 *             double * xend - x value of upper-right corner (by reference)
 *             double * yend - y value of upper-right corner (by reference)
 *  Returns:   bool - true is successful
 *  Purpose:   It uses getopt() to parse the command line.
 */
bool
parse_cmd (int argc, char *argv[], bool *chat)
{
  int option;

  /* option is the ASCII value of the character argument passed. See
   * http://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt for
   * more documentation on using getopt(). */
  while ((option = getopt (argc, argv, "hc")) != -1)
    {
      switch (option)
        {
          case 'c':         /* -c */
            *chat = true;
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
