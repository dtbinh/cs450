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
#include <unistd.h>             /* gives us getpid(), pid_t */
#include <string.h>             /* gives us strlen() */
#include <pthread.h>            /* gives us Pthreads */
#include <sys/wait.h>           /* gives us wait() */
#include <assert.h>             /* gives us assert() */

#include "pthread.h"

/* Declare global variables here */
uint32_t globvar = 0x12345678;

uint64_t base_values[] = { 17826347582943, 287192374, 7676183592,
                           1182734441283, 99283475 };
uint64_t powers[] = { 117283472, 72927192, 122836273, 97928374, 372631034 };
uint64_t modulus[] = { 8723848100237, 182734928921, 88248203487, 928374593,
                       1729473203 };

/* main entry point */
int
main(int argc, char *argv[])
{
  // int *ptr = malloc (sizeof (int));

  printf ("[%d] globvar is at %p and has value 0x%" PRIx32 "\n", getpid (),
          &globvar, globvar);

  // printf ("ptr is at %p and points to %p\n", &ptr, ptr);

  // while (1) ;

  // fork_1 ();
  // fork_2 ();
  // fork_3 ();
  // fork_4 ();
  // fork_5 ();

  // pthreads_1 ();
  // pthreads_2 ();
  // pthreads_3 ();
  // pthreads_4 ();
  // pthreads_5 ();
  // pthreads_6 ();

  return 0;
}

