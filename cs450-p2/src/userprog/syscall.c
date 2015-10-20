#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "lib/syscall-nr.h"
#include "userprog/process.h"
#include "devices/input.h"
#include <ctype.h>
#include "userprog/pagedir.h"
#include "devices/shutdown.h"
#include <stdbool.h>
static void syscall_handler (struct intr_frame *);
void syscall_init (void);

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
  for (i = 0; i < num_args; i++);
    *(va_arg (arg, uint32_t*)) = *++sp;
  va_end (arg);

  if ((uint32_t)sp >= (uint32_t)PHYS_BASE)
    thread_exit ();

}

static void
syscall_handler (struct intr_frame *f ) 
{
	//printf("syscall!\n");
//	printf("address if intr_frame-esp%d\n",f->esp);
//	printf("address of edp %d\n",f->ebp);
	uint32_t *syscall_num;
	void* rd_buf;
	const void* write_buf;
	int fdsc = -1;
	unsigned filesize = 0;
	int exit_status;
//hex_dump ( (uintptr_t)f->esp, (const void *)f->esp, (uint32_t)PHYS_BASE - (uint32_t)f->esp, true);
	
	syscall_num =(uint32_t*)f->esp;
	//printf("syscall_num %d\n",*syscall_num);
	switch(*syscall_num)
		{
			//SYS_HALT
			case SYS_HALT:
				halt();
				break;

			//SYS_EXIT
			case SYS_EXIT:
				
				parse_syscall_args(f->esp,1,&exit_status);
				f->eax = exit_status;
				//printf("exit complete");
				break;
				
			//SYS_EXEC
			case SYS_EXEC:
				break;

			//SYS_WAIT
			case SYS_WAIT:
				break;

			//SYS_CREATE
			case SYS_CREATE:
				break;

			//SYS_REMOVE
			case SYS_REMOVE:
				break;

			//SYS_OPEN
			case SYS_OPEN:
				break;

			//SYS_FILESIZE
			case SYS_FILESIZE:
				break;

			//SYS_READ

			case SYS_READ:
		
				fdsc =(int)(f->esp+4);
				rd_buf =(f->esp+8);
				filesize =*(unsigned*)(f->esp+12);
				f->eax = read(fdsc,rd_buf,filesize);
				break;
			//SYS_WRITE
			case SYS_WRITE:
				fdsc =*(int*)(f->esp+4);
				write_buf =(f->esp+8);
				filesize =*(unsigned*)(f->esp+12);
				f->eax = write(fdsc,write_buf,filesize);		
        break;
			//SYS_SEEK
			case SYS_SEEK:
					break;

			//SYS_TELL
			case SYS_TELL:
					break;

			//SYS_CLOSE
			case SYS_CLOSE:
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

void halt(void)
{
	shutdown_power_off();
}
pid_t exec(const char* cmd_line UNUSED)
{

	return -1;
}
int wait(pid_t pid UNUSED)
{
	return -1;
}
bool create(const char* file UNUSED, unsigned initial_size UNUSED)
{
	return 0;
}
bool remove (const char *file UNUSED)
{
	return -1;
}
int open (const char *file UNUSED)
{
	return -1;
}
int filesize (int fd UNUSED)
{
return -1;
}
int read (int fd, void *buffer, unsigned size)
{
	if(pagedir_get_page(thread_current()->pagedir,buffer) != NULL)
					{
						if((fd == 0))
							return input_getc();
						else
							//read(*fdsc,(void*)rd_buf,*filesize);
							printf("inside of else size is %d\n",size);
					}
				else
					printf("Invalid Memory Mapping!\n");
					thread_exit();
	return -1;
}
int write (int fd, const void *buffer, unsigned size)
{
		if(pagedir_get_page(thread_current()->pagedir,buffer) != NULL)
			{	
				if(fd == 1 )
						 putbuf(buffer,(size_t)size);
				else
				{
						printf("File Does not Exist!\n");
						process_exit();
				}	
					}
				else
					printf("Invalid Memory Mapping!\n");
					thread_exit();
	return -1;
}
void seek (int fd UNUSED, unsigned position UNUSED)
{
	
}
unsigned tell (int fd UNUSED)
{
		return 1;
}
void close (int fd UNUSED)
{
	
}

