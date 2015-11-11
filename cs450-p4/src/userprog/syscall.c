#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
void parse_syscall_args (uint32_t *sp, int num_args, ...);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* Helper function for processing a variable number of
   arguments passed on the stack.  This function demonstrates
   the C99 variable number of arguments feature (not in the
   original C specification).  It can be called like this:

        parse_syscall_args (sp, 1, &arg1);
        parse_syscall_args (sp, 2, &arg1, &arg2);

   The parameters arg1, arg2, etc., must be uint32_t. */
void
parse_syscall_args (uint32_t *sp, int num_args, ...)
{
  va_list arg;
  int i;

  va_start (arg, num_args);
  for (i = 0; i < num_args; i++)
    *(va_arg (arg, uint32_t*)) = *++sp;
  va_end (arg);

  if ((uint32_t)sp >= (uint32_t)PHYS_BASE)
    thread_exit ();
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");
  thread_exit ();
}
