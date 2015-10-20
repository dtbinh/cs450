#ifndef __CS450_SYNCH__
#define __CS450_SYNCH__

#include <unistd.h>
#include "../lib/list.h"

struct lock
  {
    int lock;
    pid_t holder;
  };

void lock_init (struct lock * lock);
void lock_acquire (struct lock * lock);
void lock_release (struct lock * lock);

struct sema
  {
    int value;
    struct list waiters;
    struct lock lock;
  };

void sema_init (struct sema *sema, int init);
void sema_down (struct sema *sema);
void sema_up (struct sema *sema);
#endif
