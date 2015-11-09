#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include <ctype.h>
#include "userprog/process.h"
#include "threads/synch.h"


void syscall_init (void);
void parse_syscall_args (uint32_t *sp, int num_args, ...);
void halt(void);
pid_t exec(const char* cmd_line);
int wait(pid_t pid);
bool create(const char* file, unsigned initial_size);
bool remove (const char *file);
int open (const char *file);
int filesize (int fd);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
void seek (int fd, unsigned position);
unsigned tell (int fd);
void close (int fd);
void exit(int status);
#endif /* userprog/syscall.h */
