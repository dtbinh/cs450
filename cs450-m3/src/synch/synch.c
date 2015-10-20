#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <sys/syscall.h>

#include "synch.h"
#include "../lib/list.h"

#define YIELD usleep((uint32_t)(rand() % 100));

/* LOCK IMPLEMENTATION */

void
lock_init (struct lock *lock)
{
  lock->holder = 0;
  lock->lock = false;
}

void
lock_acquire (struct lock *lock)
{
  int locked = true;
  pid_t tid = syscall(SYS_gettid);

  /* Use atomic cmpxchg to get lock */
  while (locked)
    {
      /* Copy the expected value "true" into register %edx. This will be the new
         value if the lock is unlocked. */
      asm volatile ("movl $1, %%edx;" : : :"%edx");

      /* Load false (unlocked) into %eax. */
      asm volatile ("movl $0, %%eax;" : : :"%eax");

      /* Compare value in %eax (false) with value in lock->lock. If they are
         equal, value in %edx (true) gets loaded into lock->lock, and %eax will
         retain its value (false). If they are not equal (lock->lock = false),
         load %edx (true) into %eax. */
      asm volatile ("lock cmpxchg %%edx, %0;" :"=m"(lock->lock) : :"%eax","%edx");

      /* If %eax = true, we failed. If %eax = false, we acquired the lock. In
         either case, store the result into locked for use in this loop. */
      asm volatile ("movl %%eax, %0" :"=r"(locked) : : );

      if (locked) YIELD;
    }

  /* Only the holder should release it */
  lock->holder = tid;
}

void
lock_release (struct lock *lock)
{
  pid_t tid = syscall(SYS_gettid);
  assert (lock->holder != tid);

  lock->lock = false;
  lock->holder = 0;
}


/* SEMAPHORE IMPLEMENTATION */

/* Structure used to build the semaphore's waiting queue. */
struct sema_elem
  {
    bool unblocked;
    struct list_elem elem;
  };

static void sema_wait_for_signal (struct sema *, struct sema_elem *);

/* Initialize the semaphore to a particular value, initialize the lock
   contained in the semaphore, and initialize the waiting list. */
void
sema_init (struct sema *sema, int init)
{
  /* Implement this here. */
  sema->value = init;
  list_init (&sema->waiters);
  lock_init (&sema->lock);
};

/* Use a lock to control access to the internals of the semaphore. After
   acquiring the lock, decrement the semaphore. If the result is negative,
   then we have to block. Our block procedure involves pushing a sema_elem
   local variable onto the semaphore's waiting list. Thus, the waiting list
   consists of local variables that are used to link threads together. Each
   thread has tried to down the semaphore and has had to block. After adding
   the sema_elem to the waiting list, call sema_wait_for_signal() to relinquish
   the lock until it is our turn. We must release the semaphore lock at the end
   of this function. */
void
sema_down (struct sema *sema)
{
  struct sema_elem se; /* Used for blocking the current thread. */

  if (sema->value < 0)
    {
      se.unblocked = false;
      list_push_back (&sema->waiters, &se.elem);
      sema_wait_for_signal (sema, &se);
    }
}

/* Used to emulate a condition variable. The caller is trying to acquire the
   semaphore sema. It has a local sema_elem variable that gets added to the
   semaphore's waiting list. When the semaphore has been upped and passed to
   this thread, se->unblocked will be set to true. So this function must loop
   while se->unblocked is false, releasing the semaphore's lock, yielding, then
   acquiring the lock again. Note that the thread has previously acquired the
   semaphore's lock before calling this function, and it must still hold it when
   the function returns. */
static void
sema_wait_for_signal (struct sema *sema, struct sema_elem *se)
{
  /* implementation here */
  pid_t tid = syscall(SYS_gettid);
  assert (sema->lock.holder == tid);

  while (se->unblocked == false)
    {
      lock_release (&sema->lock);
      YIELD;
      lock_acquire (&sema->lock);
    }
}

/* Up the semaphore. You should acquire the lock then increment the value.
   After the increment, check the waiting list to see if a thread is waiting
   on the semaphore. If so, remove the first waiter and unblock its sema_elem.
   Be sure to release the semaphore lock. */
void
sema_up (struct sema *sema)
{
  if (! list_empty (&sema->waiters))
    {
      struct list_elem *e = list_pop_front (&sema->waiters);
      struct sema_elem *se = list_entry (e, struct sema_elem, elem);
      list_remove (e);
      se->unblocked = true;
    }
}
