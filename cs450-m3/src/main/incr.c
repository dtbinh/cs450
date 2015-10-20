/* chat.c
 * ------
 *  Author:   <<YOUR NAME HERE>>
 *  Purpose:  This file demonstrates semaphores
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
#include <time.h>		/* usleep() */
#include <sys/syscall.h>

#include "../synch/synch.h"
#include "incr.h"

#define NUM_THREADS 10
#define ROWS 10
#define COLS 10

#define YIELD usleep((uint32_t)(rand() % 10000));

/* Local function declarations */
void * scan_for_updates (void * _args);

/* Declare global variables here */
int array[ROWS][COLS];
uint8_t num_writes = 0; /* Do 5 updates only, then exit */

/* Another child thread function. Here, we are going to use semaphores to
   control the timing. The child will down the semaphore before starting to
   print messages to the screen. */
void *
scan_for_updates (void * _args)
{
  int local[COLS];
  uint8_t i;
  uint32_t sum;
  long tid = (long)_args;
  uint8_t num_diff = 0; /* each time through, should be 0 or COLS */

  printf ("Child %ld is starting\n", tid);
  for (i = 0; i < COLS; i++) local[i] = 0;

  /* Include implementation here */
  while (num_writes < 5)
    {
      num_diff = 0;
      for (i = 0; i < COLS; i++)
        if (local[i] != array[tid][i]) num_diff++;

      if (num_diff > 0)
        {
          sum = 0;
          for (i = 0; i < COLS; i++) sum += local[i] = array[tid][i];
          printf ("[%ld] updated sum: %" PRIu32 "\n", syscall(SYS_gettid), sum);
        }
    }

  pthread_exit (0);
}

/* Parent thread demonstrating the use of semaphores for timing control. The
   child should pause until the parent says to go. */
void
incr_all ()
{
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  void *status;
  long t;
  int rc;
  uint8_t i, j;

  for (i = 0; i < ROWS; i++)
    for (j = 0; j < COLS; j++)
      array[i][j] = i+j;

  printf ("Parent is starting\n");
  printf ("Array before:\n");
  for (i = 0; i < ROWS; i++)
    {
      for (j = 0; j < COLS; j++)
        printf ("%4d", array[i][j]);
      printf ("\n");
    }

  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  /* Create and initialize semaphores here */

  /* Launch the child thread and pass it the semaphores */
  for (t = 0; t < NUM_THREADS; t++)
    {
      rc = pthread_create (&threads[t], &attr, scan_for_updates, (void *)t);
      if (rc != 0)
        {
          fprintf (stderr, "ERROR: Failed to create thread %d\n", rc);
          pthread_exit (NULL);
        }
    }
  pthread_attr_destroy (&attr);

  /* Include implementation here */
  for (t = 0; t < 5; t++)
    {
      printf ("[%ld] starting update...\n", syscall(SYS_gettid));

      printf ("sleeping...\n");
      usleep (500000);
    }

  /* Now, wait for all threads to finish */
  for (t = 0; t < NUM_THREADS; t++)
    {
      rc = pthread_join (threads[t], &status);
      if (rc != 0)
        {
          fprintf (stderr, "ERROR: Failed to join thread %d\n", rc);
          pthread_exit (NULL);
        }
    }

  printf ("Array after:\n");
  for (i = 0; i < ROWS; i++)
    {
      for (j = 0; j < COLS; j++)
        printf ("%4d", array[i][j]);
      printf ("\n");
    }
  pthread_exit (NULL);
}
