/* pipe.c
 * ------
 *  Author:   <<YOUR NAME HERE>>
 *  Purpose:  This file demonstrates how to use a pipe()
 */

/* Library includes here.  */
#include <stdio.h>              /* standard I/O */
#include <stdint.h>             /* uint32_t and similar types */
#include <stdbool.h>            /* Boolean types */
#include <inttypes.h>           /* PRIu** format strings */
#include <sys/wait.h>           /* for waitpid() */
#include <stdlib.h>             /* gives us malloc() */
#include <string.h>             /* memset() and strcmp() */
#include <unistd.h>             /* fork(), getpid(), execv() */

/* Local function declarations */
//void child_tokenizer (int * read_fd, int * write_fd);
void child_tokenizer (int read_fd, int write_fd);
void parent_stdin (int read_fd, int write_fd);

/* Declare global variables here */

#define BUFFER_SIZE 100
#define DONE_TOKEN "Finito"

void
build_pipe ()
{
  pid_t pid;
  int status;
  int fd[2][2];

  status = pipe(fd[0]);
  if (status == -1)
    {
      fprintf (stderr, "Failed to build pipe\n");
      exit (-1);
    }

  status = pipe(fd[1]);
  if (status == -1)
    {
      fprintf (stderr, "Failed to build pipe\n");
      exit (-1);
    }

  pid = fork ();

  if (pid < 0)
    {
      fprintf (stderr, "Failed to fork\n");
      exit (-1);
    }

  if (pid == 0)
    {
      /* Child reads from fd[0][0], writes to fd[1][1] */
      close (fd[0][1]);
      close (fd[1][0]);
      child_tokenizer (fd[0][0], fd[1][1]);
      exit (0);
    }

  /* Parent writes to fd[0][1], reads from fd[1][0] */
  close (fd[0][0]);
  close (fd[1][1]);
  parent_stdin (fd[1][0], fd[0][1]);

  printf ("[%d] Parent is done\n", getpid ());

}

void
parent_stdin (int read_fd, int write_fd)
{
  char buffer[BUFFER_SIZE];
  ssize_t nbytes;

  strncpy (buffer, "Hello, my name is Inigo Montoya", BUFFER_SIZE-1);
  buffer[BUFFER_SIZE-1] = '\0';
  printf ("[%d] Parent is writing '%s'\n", getpid(), buffer);
  nbytes = write (write_fd, buffer, BUFFER_SIZE);
  if (nbytes <= 0)
    {
      fprintf (stderr, "[%d] Failed to write to pipe\n", getpid ());
      return;
    }

  while (true)
    {
      nbytes = read (read_fd, buffer, BUFFER_SIZE);
      if (nbytes <= 0) break;
      if (strcmp (buffer, DONE_TOKEN) == 0) break;
      printf ("[%d] Parent received '%s'\n", getpid(), buffer);
    }
}

void
child_tokenizer (int read_fd, int write_fd)
{
  char buffer[BUFFER_SIZE];
  char outbuf[BUFFER_SIZE];
  ssize_t nbytes;
  char * token;
  char * save;

  printf ("[%d] Child is waiting to read...\n", getpid());
  nbytes = read(read_fd, buffer, BUFFER_SIZE);
  if (nbytes <= 0)
    {
      printf ("[%d] Child received nothing\n", getpid());
      exit (-1);
    }

  printf ("[%d] Child received '%s'\n", getpid(), buffer);

  /* Do not include this printf in your write() counting: */
  // printf ("[%d] buffer is at %p\n", getpid (), buffer);

  /* get first token */
  token = strtok_r (buffer, " ", &save);
  while (token != NULL)
    {
      /* Do not include this printf in your write() counting: */
      // printf ("[%d] token = %p, save = %p\n", getpid (), token, save);

      /* Put the message into a buffer */
      snprintf (outbuf, sizeof (outbuf)-1, "Token: ");
      strncat (outbuf, token, BUFFER_SIZE - 7);
      outbuf[BUFFER_SIZE-1] = '\0';

      /* Send the buffer back to the parent */
      nbytes = write (write_fd, outbuf, BUFFER_SIZE);
      if (nbytes <= 0)
        {
          fprintf (stderr, "[%d] Failed to write to pipe\n", getpid ());
          goto done;
        }

      /* Get the next token */
      token = strtok_r (NULL, " ", &save);
    }

  strncpy (outbuf, DONE_TOKEN, BUFFER_SIZE-1);
  outbuf[BUFFER_SIZE-1] = '\0';
  nbytes = write (write_fd, outbuf, BUFFER_SIZE);
  if (nbytes <= 0)
    fprintf (stderr, "[%d] Failed to write to pipe\n", getpid ());

  printf ("[%d] Child received '%s'\n", getpid (), buffer);

done:
  close (read_fd);
  close (write_fd);
}
