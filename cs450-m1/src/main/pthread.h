#ifndef __CS450_PTHREAD__
#define __CS450_PTHREAD__

#include <stdint.h>

/* Local function declarations */
void fork_1 (void);
void fork_2 (void);
void fork_3 (void);
void fork_4 (void);
void fork_5 (void);

void pthreads_1 (void);
void pthreads_2 (void);
void pthreads_3 (void);
void pthreads_4 (void);
void pthreads_5 (void);
void pthreads_6 (void);

typedef
struct modexp_args
{
  uint64_t base;
  uint64_t power;
  uint64_t modulus;
} modexp_t;

void modexp (modexp_t *);
void * say_hi (void *);
void modular_exponentiation (uint64_t, uint64_t, uint64_t);
void * mod_exp_thread (void *);

/* Declare global variables here */
extern uint32_t globvar;

extern uint64_t base_values[];
extern uint64_t powers[];
extern uint64_t modulus[];

#define NUM_THREADS 5

#endif
