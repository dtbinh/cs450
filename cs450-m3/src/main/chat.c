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

#include "../synch/synch.h"
#include "chat.h"

/* Local function declarations */
void * child (void * _args);

/* Declare global variables here */

struct to_from_sema
  {
    struct sema *to_parent;
    struct sema *to_child;
  };

/* Another child thread function. Here, we are going to use semaphores to
   control the timing. The child will down the semaphore before starting to
   print messages to the screen. */
void *
child (void * _args)
{
  /* Include implementation here */
  printf ("[CHILD] Hello parent, I'm starting like you asked.\n");
  printf ("[CHILD] Parent, what is the ultimate answer to life, the universe, and everything?\n");
  printf ("[CHILD] I'm glad you think it's a simple answer. What is it?\n");
  printf ("[CHILD] Really? The answer is 43?\n");
  printf ("[CHILD] Oh, I heard wrong. It's 42. Thanks! I'm leaving now\n");

  pthread_exit (0);
}

/* Parent thread demonstrating the use of semaphores for timing control. The
   child should pause until the parent says to go. */
void
start_chat ()
{
  pthread_t thread;
  pthread_attr_t attr;
  void *status;
  int rc;
  struct sema to_parent;
  struct sema to_child;
  struct to_from_sema tfs;

  printf ("Parent is starting\n");
  pthread_attr_init (&attr);
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  /* Launch the child thread and pass it the semaphores */
  rc = pthread_create (&thread, &attr, child, (void *)&tfs);
  if (rc != 0)
    {
      fprintf (stderr, "ERROR: Failed to create thread %d\n", rc);
      pthread_exit (NULL);
    }
  pthread_attr_destroy (&attr);

  /* Include implementation here */
  printf ("[PARENT] Child, ready, set, go!\n");
  printf ("[PARENT] Good, you can follow directions.\n");
  printf ("[PARENT] Oh, you've seen my Douglas Adams books. It's a simple answer, really.\n");
  printf ("[PARENT] 42, of course.\n");
  printf ("[PARENT] No, please clean out your ears. It's 42.\n");
  printf ("[PARENT] Have fun storming the castle!\n");

  rc = pthread_join (thread, &status);
  if (rc != 0)
    {
      fprintf (stderr, "ERROR: Failed to join thread %d\n", rc);
      pthread_exit (NULL);
    }
  printf ("Outta here...\n");
  pthread_exit (NULL);
}
