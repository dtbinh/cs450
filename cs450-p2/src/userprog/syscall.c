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
static void syscall_handler (struct intr_frame *);
void parse_syscall_args (uint32_t *sp, int num_args, ...);
int read (int fd, void *buffer, unsigned size);
int write (int fd,void* const buffer,unsigned size);
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
	printf("syscall!\n");
//	printf("address if intr_frame-esp%d\n",f->esp);
//	printf("address of edp %d\n",f->ebp);
	uint32_t *syscall_num;
	char* rd_buf = NULL;
	const char* write_buf = NULL;
	int* fdsc = NULL;
	uint16_t* filesize;


	syscall_num =(uint32_t*)f->esp;
	//printf("syscall_num %d\n",*syscall_num);
	switch(*syscall_num)
		{
			//SYS_HALT
			case 0:
				break;
			//SYS_EXIT
			case 1:
				//sys_status = ((int)(f->esp+4));
			
				process_exit();
				printf("exit complete");
				break;
				
			//SYS_EXEC
			case 2:
				break;

			//SYS_WAIT
			case 3:
				break;

			//SYS_CREATE
			case 4:
				break;

			//SYS_REMOVE
			case 5:
				break;

			//SYS_OPEN
			case 6:
				break;

			//SYS_FILESIZE
			case 7:
				break;

			//SYS_READ

			case 8:
		
				printf("inside of sys_read\n");							
				parse_syscall_args(f->esp,3,fdsc,&rd_buf,&filesize);
				if(*fdsc == 0)
					f->eax = input_getc();
				else
					read(*fdsc,(void*)rd_buf,*filesize);
				break;
			//SYS_WRITE
			case 9:
				printf("inside of write\n");
				parse_syscall_args(f->esp,3,fdsc,&write_buf,&filesize);
				printf("fdsc_write %d\n",*fdsc);	
				if(*fdsc == 1)
					putbuf(write_buf,(size_t)filesize);
				else
					{
						printf("File Does not Exist!\n");
						process_exit();
					}
				break;

			//SYS_SEEK
			case 10:
					break;

			//SYS_TELL
			case 11:
					break;

			//SYS_CLOSE
			case 12:
					break;

			//SYS_MNMAP
			case 13:
					break;

			//SYS_MUMAP
			case 14:
					break;

			//SYS_CHDIR
			case 15:
					break;

			//SYS_MKDIR
			case 16:
					break;

			//SYS_READDIR
			case 17:
					break;

			//SYS_ISDIR
			case 18:
					break;

			//SYS_ISNUMBER
			case 19:
					break;
		}
	//hexdump here
  thread_exit ();
}


/*
int
read(int fd,void *buffer,unsigned size)
{
//read from file descriptor



// use input_getc to read the file into buffer
// then use buffer to get size	


}

int 
write(int fd,void* const buffer,unsigned size)
{





}*/
