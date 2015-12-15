/* command line parsing routine */

#include <stdio.h>              /* standard I/O */
#include <unistd.h>             /* declares getopt() */
#include <stdlib.h>             /* declares exit() */
#include <ctype.h> 				/* declares isprint()*/
#include "parse.h"              /* prototypes for exported functions */

/* Show the help message that explains command-line options */
void
show_usage (void)
{
  printf ("Usage: mandel [options]\n");
  printf ("Options:\n");
  printf ("  -m F         Run Mandelbrot with parameters in file F\n");
  printf ("  -s F         Sort the data set with parameters in file F\n");
  printf ("  -q F         Query the data set with parameters in file F\n");
  printf ("  -h           Print this message and exit\n");
}

/* parse_command_line -- parses the command-line arguments using getopt()    */
int
parse_command_line (int argc, char **argv,char **mandel_params,
                  char **sort_params,char **query_params,char *mode_select)
{
  int option;
  while ((option = getopt (argc, argv, "m:s:q:h")) != -1)
    {
      switch (option)
        {
        case 'h':
          show_usage ();
          exit (0);
          break;
				case 'm':
					*mode_select = 'm';
		  			*mandel_params = optarg;	
		  			break;

				case 's':
		  			*mode_select = 's';
		  			if(optarg != NULL)
		  			{
		    				*sort_params = optarg;	
	  	  			}
		  			break;	
				
				case 'q':
		  			*mode_select = 'q';	
		  			if(optarg != NULL)
	 				{
	  					*query_params = optarg;	
	 				}	
	 		 		break;
		
			case '?':
			 		if(optopt == 'm' || optopt == 's' || optopt == 'q')
						fprintf(stderr,"Option %c requires an argument\n",optopt);					else if (isprint (optopt))
            fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          else
            fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
		  		return 1;
      		break;  
			
			default:
					abort();
          break;
        }
    }
	return 0;
}


