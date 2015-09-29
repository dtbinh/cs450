/* FORK 4 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdlib.h>             /* gives us exit() */
#include <unistd.h>             /* gives us fork() */
#include <stdint.h>             /* uint32_t and similar types */
#include <inttypes.h>           /* PRIu** format strings */

#include "pthread.h"

void
fork_4 (void)
{
  uint64_t base_value = 17826347582943;
  uint64_t power = 117283472;
  uint64_t modulus = 8723848100237;

  pid_t pid = fork ();
  if (pid < 0) return;
  if (pid == 0)
    {
      printf ("[%d] Calculating %" PRIu64 "^%" PRIu64 " mod %" PRIu64 "\n",
              getpid (), base_value, power, modulus);
      modular_exponentiation (base_value, power, modulus);
    }
  printf ("[%d] Exiting\n", getpid ());
}
