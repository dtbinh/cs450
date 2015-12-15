#include "userprog/syscall.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include "lib/syscall-nr.h"
#include "userprog/process.h"
#include "devices/input.h"
#include <ctype.h>
#include "userprog/pagedir.h"
#include "devices/shutdown.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

struct semaphore write_allowed;
struct lock mutex;
size_t reader_count;
struct lock list_lock;
static void syscall_handler (struct intr_frame *);

// GLOBALS
//struct dead_thread_t graveyard[64];
//struct list limbo;
void
syscall_init (void) 
{
	sema_init(&write_allowed, 1);
	lock_init (&mutex);
	lock_init(&list_lock);
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
syscall_handler (struct intr_frame *f ) 
{
	if(f->esp >= (void *)0xbfffffe8) /* Lowest possible value for a stack to start at */
	{
		exit(-1);
	}

	if(f->esp <= (void *)(0x08048000)) /* Bad esp address */
	{
	 	 exit(-1);
	}
	uint32_t *syscall_num;
	void* rd_buf;
	const void* write_buf;
	int fdsc = -1;
	unsigned file_size = 0;
	int exit_status = -1;
	const char* fname;
	const char* cmd;
	pid_t pid;	
	syscall_num =(uint32_t*)f->esp;
	switch(*syscall_num)
	{
			//SYS_HALT
			case SYS_HALT:
				halt();
				break;

			//SYS_EXIT
			case SYS_EXIT:
				parse_syscall_args(f->esp,1,&exit_status);	
				exit(exit_status);
				break;
				
			//SYS_EXEC
			case SYS_EXEC:
				parse_syscall_args(f->esp,1,&cmd);
				f->eax = exec(cmd);
				break;

			//SYS_WAIT
			case SYS_WAIT:
				parse_syscall_args(f->esp,1,&pid);
				wait((tid_t)pid);
				break;

			//SYS_CREATE
			case SYS_CREATE:
				parse_syscall_args(f->esp,2,&fname,&file_size);
				f->eax = create(fname,file_size);
				break;

			//SYS_REMOVE
			case SYS_REMOVE:
				parse_syscall_args(f->esp,2,&fname,&file_size);
				f->eax = remove(fname);
				break;

			//SYS_OPEN
			case SYS_OPEN:
				parse_syscall_args(f->esp,1,&fname);
				f->eax = open(fname);
				break;

			//SYS_FILESIZE
			case SYS_FILESIZE:
				parse_syscall_args(f->esp,1,&fdsc);
				f->eax = filesize(fdsc);	
				break;

			//SYS_READ

			case SYS_READ:
		
				parse_syscall_args(f->esp, 3, &fdsc,&rd_buf, &file_size);
				f->eax =(uint32_t) read(fdsc,rd_buf,file_size);
				break;
			//SYS_WRITE
			case SYS_WRITE:
				parse_syscall_args(f->esp, 3, &fdsc,&write_buf, &file_size);
				f->eax = (uint32_t) write(fdsc,write_buf, file_size);		
        break;
			//SYS_SEEK
			case SYS_SEEK:
					parse_syscall_args(f->esp,1,&fdsc,&file_size);
					seek(fdsc,file_size);	
					break;

			//SYS_TELL
			case SYS_TELL:
					parse_syscall_args(f->esp,1,&fdsc);
					f->eax = tell(fdsc);
					break;

			//SYS_CLOSE
			case SYS_CLOSE:
					parse_syscall_args(f->esp,1,&fdsc);
					close(fdsc);
					break;

			//SYS_MMAP
			case SYS_MMAP:
					break;

			//SYS_MUMAP
			case SYS_MUNMAP:
					break;

			//SYS_CHDIR
			case SYS_CHDIR:
					break;

			//SYS_MKDIR
			case SYS_MKDIR:
					break;

			//SYS_READDIR
			case SYS_READDIR:
					break;

			//SYS_ISDIR
			case SYS_ISDIR:
					break;

			//SYS_ISNUMBER
			case SYS_INUMBER:
					break;
		}
	//hexdump here
}

void 
halt(void)
{
	shutdown_power_off();
}


pid_t
exec(const char* cmd_line)
{
	//char* cmd_copy = ""; // copy just to be sure there aren't race conditions
	//strlcpy(cmd_copy,cmd_line,strlen(cmd_line));
	return (pid_t)process_execute(cmd_line);
}

/*
TODO: Sketch thoughts for wait
Use conditions
call wait.
wait has pid.
from there, a condition might be helpful. ask kirkpams
* potentially use cond_wait to wait to check exit status of
* child processes. Then use cond_signal to run the next process(not sure?)
* if the event that a parent dies, then use cond_broadcast to terminate orphans
*
*/
int
wait(pid_t pid)
{
	struct thread* current;
  struct list_elem *d;
  struct list_elem *e;
	struct thread* temp;
	dead_thread_t *dead_child;
	current = thread_current();
	thread_current()->is_waiting = false;
	lock_acquire(&list_lock);	
	for (e = list_begin (&current->limbo); e != list_end (&current->limbo); e = list_next (e))
		{
			dead_child = list_entry(e,dead_thread_t,elem);

			cond_init(&(dead_child->cond));
			lock_init(&(dead_child->dead_lock));
			lock_acquire(&(dead_child->dead_lock));

			if (pid == dead_child->tid)
				{
					return dead_child->exit_status;
				}
		}

	for (d = list_begin (&current->youngins); d != list_end (&current->youngins); d = list_next (d))
		{
			temp = list_entry (d, struct thread, child_elem);
			
			cond_init(&(temp->cond));
			lock_init(&(temp->proc_lock));
			lock_acquire(&(temp->proc_lock));
			if(pid == temp->tid)
				{						
					if(!list_empty(&current->youngins))
					{
						cond_signal(&(temp->cond),&(temp->proc_lock));
						return wait((pid_t)temp->tid);
					}
					if(temp->parent != NULL)
						{
							cond_wait(&(temp->cond),&(temp->proc_lock));	 
							lock_release(&(temp->proc_lock));
						}
				}
		}
	 return -1;
}

bool
create(const char* file, unsigned initial_size )
{
	bool file_create = false;
	if(pagedir_get_page(thread_current()->pagedir, file) == NULL)
	{
		exit(-1);
		return 0;
	}
	if(file != NULL)
	{	
		file_create = filesys_create(file,initial_size);
		//call open
		//call 
	}
	return file_create;
}

bool
remove (const char *file)
{
	bool file_remove;
	file_remove = filesys_remove(file);
	return file_remove;
}
int
open (const char *file )
{
	int ii;
	struct file* fp = NULL;

	if(pagedir_get_page(thread_current()->pagedir, file) == NULL)
	{
		exit(-1);
		return 0;
	}

	if(file == NULL) return 0;

	else
	{
		fp = filesys_open(file);
		if (fp == NULL) return -1; //error check if file dne	
	
		for(ii = 2; ii < 128;ii++)
		{
			if(thread_current()->files[ii] == NULL)
			{
				thread_current()->files[ii] = fp;
				break;
			}
		}
		return ii;
	}
}
int
filesize (int fd )
{
	int fsize = 0;
  int ii;
	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{
				if(fd == ii)
					{
						fsize = file_length(thread_current()->files[ii]);
					}
		}
	  ii++;
  //seek to end, tell position, seek to beginning
	return fsize;
}
int
read (int fd, void *buffer, unsigned size)
{
	int file_size = 0;
	int ii;
	if(is_user_vaddr((const void*)buffer) == 0 ||
									 pagedir_get_page(thread_current()->pagedir,buffer) == NULL)
		return 0;
		
		if(fd == 0)
			{
				lock_acquire(&mutex);
				reader_count++;
				if(reader_count == 1)	sema_down(&write_allowed);
				lock_release(&mutex);
				
				file_size = input_getc();
				
				lock_acquire(&mutex);
				reader_count--;
				if(reader_count == 0) sema_up(&write_allowed);
				lock_release(&mutex);
			}

	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{	
			if(fd == ii)
				{
						lock_acquire(&mutex);
						reader_count++;
						if(reader_count == 1)	sema_down(&write_allowed);
						lock_release(&mutex);
						
						file_size = file_read(thread_current()->files[ii],buffer,size);
						
						lock_acquire(&mutex);
						reader_count--;
						if(reader_count == 0) sema_up(&write_allowed);
						lock_release(&mutex);
				}
		}
	return file_size;
}

int 
write (int fd, const void *buffer, unsigned size)
{
	int status = -1;
	int ii;
	if(pagedir_get_page(thread_current()->pagedir,buffer) == NULL)
	{
		exit(-1);
		return 0;
	}
	if(fd == 0) return 0;	
		
	if(fd == 1)
		{
			sema_down(&write_allowed);
			putbuf((const char*)buffer,size);
			sema_up(&write_allowed);
			status = 0;
		}
	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{	
				if(fd == ii)
				{
					sema_down(&write_allowed);
					status =	file_write(thread_current()->files[ii],buffer,size);
					sema_up(&write_allowed);	
				}
		}
	return status;
}


void 
seek (int fd, unsigned position)
{
	int ii;
	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{
				if(fd == ii)
					{
						file_seek(thread_current()->files[ii],position);
					}
		}
}


unsigned
tell (int fd)
{
	int size = 0;
	int ii;
	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{
				if(fd == ii)
					{
						size = file_tell(thread_current()->files[ii]);
					}
		}
	return size;
}

void
close (int fd)
{
	int ii;
	for(ii = 2;ii < 128 && thread_current()->files[ii] != NULL;ii++)
		{
				if(fd == ii)
					{
						file_close(thread_current()->files[ii]);
						thread_current()->files[ii] = NULL;
					}
		}
}

void 
exit(int status)
{

	dead_thread_t* dead;
  struct list_elem *d;
	struct thread* temp;

	dead = (dead_thread_t*)malloc(sizeof(dead_thread_t));
	dead->exit_status = status;
	dead->tid = thread_current()->tid;
//	printf("status %d\n",dead->tid);
	lock_init(&(thread_current()->parent->proc_lock));
	lock_acquire(&(thread_current()->parent->proc_lock));
	list_push_back(&(thread_current()->parent->limbo),&(dead->elem));
	
	for (d = list_begin (&thread_current()->parent->youngins) ;
									d != list_end (&thread_current()->parent->youngins);d = list_next (d))
	{
					temp = list_entry (d, struct thread, child_elem);	
					if(temp->tid == thread_current()->tid)
					{
							list_remove(d);
					}									
	}
	lock_release(&(thread_current()->parent->proc_lock));
	thread_current()->exit_status = status;
	thread_exit();
}
