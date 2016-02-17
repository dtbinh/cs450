/* ************************************************************************* *
 * main.c                                                                    *
 * ------                                                                    *
 *  Author:   YOUR NAME HERE                                                 *
 *  Purpose:  This is a template for project 1.                              *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.  For documentation of standard C library           *
 * functions, see the list at:                                               *
 *   http://pubs.opengroup.org/onlinepubs/009695399/functions/contents.html  *
 * ************************************************************************* */
 
#include <stdio.h>              /* standard I/O */
#include <stdbool.h>            /* bool types */
#include <string.h> 						/*strings */
#include <stdlib.h>

#include "parse.h"              /* command line parser */
#include "../mandel/mandel.h"
#include "../msort/msort.h"

bool float_match (float a, float b);

/* main -- main entry point into the program. */
int
main (int argc, char **argv)
{
  /* Your implementation here. */
	char mode_select = NULL;
	char* mandel_params = NULL;
	char* sort_params = NULL;
	char* query_params = NULL;	

	parse_command_line(argc,argv,&mandel_params,&sort_params,&query_params,&mode_select);
	switch(mode_select)
	{
		case 'm':
		if(mandel_params != NULL)
		{
				//printf("inside of mandelbrot main_funct\n");
				run_mandelbrot(mandel_params,NULL);	
		}
		else
		{
				fprintf(stderr,"invalid set passed!%s\n",mandel_params);
		}
		break;	
		case 's':
		if(sort_params != NULL)
		{
			run_msort(sort_params, NULL);;
		}
		else
		{
				fprintf(stderr,"invalid set passed!%s",mandel_params);
		}
		break;

		case 'q':
		if(query_params != NULL)
		{
			printf("insert query implementation here\n");
		}
		else
		{
				fprintf(stderr,"invalid set passed!%s",mandel_params);
		}
		break;

	}
  return 0;
}

/* checks to see if two floating-point values are within 0.001 of each other */
bool
float_match (float a, float b)
{
  if (b > a) return float_match (a, b);

  if (a - b < 0.001) return true;
  else return false;
}

