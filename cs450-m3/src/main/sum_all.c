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

#include "../synch/synch.h"
#include "sum_all.h"

#define BUFFER_SIZE 10
#define NUM_THREADS 100
#define NUMS_PER_THREAD 50

/* Local function declarations */
void * gen_nums (void * _args);

/* Declare global variables here */
int buffer[BUFFER_SIZE];
uint8_t next_in = 0;
uint8_t next_out = 0;

/* Another child thread function. Here, we are going to use semaphores to
   control the timing. The child will down the semaphore before starting to
   print messages to the screen. */
void *
gen_nums (void * _args)
{
  uint8_t i;
  int num;
  long tid = (long)_args;

  printf ("Child %ld is starting\n", tid);

  /* Include implementation here */
  for (i = 0; i < NUMS_PER_THREAD; i++)
    {
      usleep (rand() % 1000000);
      num = rand() % 100;

      printf ("[%ld] sending %d\n", tid, num);
    }

  pthread_exit (0);
}

/* Parent thread demonstrating the use of semaphores for timing control. The
   child should pause until the parent says to go. */
void
sum_all ()
{
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  void *status;
  long t;
  int rc;
  int numbers[20];
  int sum = 0;
  uint8_t local_index = 0;
  int num;

  printf ("Parent is starting\n");
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  /* Create and initialize semaphores here */

  /* Launch the child thread and pass it the semaphores */
  for (t = 0; t < NUM_THREADS; t++)
    {
      rc = pthread_create (&threads[t], &attr, gen_nums, (void *)t);
      if (rc != 0)
        {
          fprintf (stderr, "ERROR: Failed to create thread %d\n", rc);
          pthread_exit (NULL);
        }
    }
  pthread_attr_destroy (&attr);

  /* Include implementation here */
  for (t = 0; t < NUM_THREADS * NUMS_PER_THREAD; t++)
    {
      /* pull out number, add it to local copy, add to sum */

      numbers[local_index++] = num;
      sum += num;

      /* If we're at the 20th number, get the lock and dump */
      if (local_index == 20)
        {
          /* Make sure all of these messages are printed together */
          printf ("Dumping array: ");
          for (local_index = 0; local_index < 20; local_index++)
            printf ("%d ", numbers[local_index]);

          printf ("\nCurrent sum: %d\n", sum);
          local_index = 0;
        }
    }

  printf ("Total is %d\n", sum);

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

  pthread_exit (NULL);
}
