/* say_hi.c
 * --------
 *  Author:   <<YOUR NAME HERE>>
 *  Purpose:  This file demonstrates locks
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
#include <sys/syscall.h>	/* gives up TID */

#include "../synch/synch.h"

#define NUM_THREADS 10
#define INCRS 1000000

/* Local function declarations */
void * say_hi (void *);

/* Declare global variables here */
uint32_t value = 12345678;

struct lock l;

/* Function that is run in a separate thread, created by lock_and_say_hi().
   Each instance just repeatedly decrements and increments a shared variable,
   thus demonstrating race conditions. As our code just increments then
   decrements, it is tempting to think that it should just return the original
   value. But it doesn't. */
void *
say_hi (void * _args)
{
  uint32_t i;

  //lock_acquire (&l);
  for (i = 0; i < INCRS; i++)
    {
      //lock_acquire (&l);
      value++;
      value--;
      //lock_release (&l);
    }
  //lock_release (&l);

  pthread_exit (0);
}

/* Demonstrates how to use a lock. This function creates NUM_THREADS (default
   is 10) and passes the lock to each of them. That lock ensures the threads
   will have mutual exclusive access to the shared resource. */
void
lock_and_say_hi ()
{
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  int rc;
  uint32_t t;
  void *status;

  printf ("Value = %"PRIu32"\n", value);

  lock_init (&l);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  /* Create the threads, passing the lock holder struct to each */
  for (t = 0; t < NUM_THREADS; t++)
    {
      rc = pthread_create (&threads[t], &attr, say_hi, (void *)t);
      if (rc != 0)
        {
          fprintf (stderr, "ERROR: Failed to create thread %d\n", rc);
          pthread_exit (NULL);
        }
    }

  pthread_attr_destroy (&attr);

  for (t = 0; t < NUM_THREADS; t++)
    {
      rc = pthread_join (threads[t], &status);
      if (rc != 0)
        {
          fprintf (stderr, "ERROR: Failed to join thread %d\n", rc);
          pthread_exit (NULL);
        }
    }

  printf ("Value = %"PRIu32"\n", value);
  pthread_exit (NULL);
}
