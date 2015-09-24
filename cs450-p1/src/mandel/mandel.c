/* Unithreaded implementation of the Mandelbrot set calculation */

#include <stdio.h>              /* standard I/O */
#include <stdint.h>             /* C99 integer types */
#include <stdlib.h> 						/* Standard C Library */
#include <assert.h>             /* declares assert() */
#include <inttypes.h>           /* more integer datatypes */
#include <string.h>             /* strings */
#include <math.h>               /* fabs */
#include <pthread.h>            /* Pthreads */
#include <unistd.h>

#include "tga.h"                /* TGA image creation */
#include "mandel.h"             /* Mandelbrot header */

/* Iterate in the Mandelbrot set 500 times (do not change) */
const int NUM_ITERATIONS = 500;


/* Static helper functions. You can change these if needed. */
static void calc_pixel (double x, double y, uint8_t * r, uint8_t * g,
                        uint8_t *b, double *v);
static void calc_mandelbrot (uint8_t * pic, double xstart, double xend,
                             double ystart, double yend, double xstep,
                             double ystep, uint32_t height, uint32_t width);
void calc_mandelbrot_multi(void* args);

/* Entry point to calculate the Mandelbrot set. Do not change the interface */
int
run_mandelbrot (char *params, index_value ** _array)
{
	printf("Running Mandlebrot\n");	

  	//Structure to contain the arguments for the pthread
 	arg_struct *args;

  	//File parsing variables
	char* calc_params[8];
  	char* calc_duplicate; //Duplicate of the string in the file
  	char *file_str = NULL;

  	uint8_t rc = 0;
  	uint8_t file_size;
  	uint8_t ii;
  	uint8_t num_threads;
	double y_chunk;

  	void *status = NULL;

  	//Allocate the argument structure here
 	args =(arg_struct*)malloc(sizeof(arg_struct));

  	//Begin parsing the file
  	FILE * fp;
	fp = fopen(params,"r"); //Open the file
	fseek (fp,0,SEEK_END);  //Begin reading the file
	file_size = ftell(fp);  //Read how many bytes the file is
	rewind(fp);
	file_str = malloc(file_size); //Allocate the space for the file string
  	fgets(file_str, file_size, fp); //Read the file string

	calc_duplicate = strdup(file_str); //Get a duplicate of the file string to manipulate

  	/*Assign the parameters for the image appropriately
    		For reference, calc_params[k] where k is: 
      			0 = file name 
      			1 = image width
      			2 = image height
      			3 = lower left x
      			4 = upper right x
      			5 = lower left y
      			6 = upper right y
      			7 = number of threads
  	*/ 
	for(ii = 0;ii < 8;ii++)
	{
		calc_params[ii] = strsep(&calc_duplicate,",");
	}
 
  	//Put the parameters inside the argument struct
  	args->x_llcoord = atof(calc_params[3]); 
  	args->y_llcoord = atof(calc_params[5]); 
  	args->x_upcoord = atof(calc_params[4]); 
  	args->y_upcoord = atof(calc_params[6]); 
  	args->width = atof(calc_params[1]);
  	args->height = atof(calc_params[2]);
  	args->xstep = (fabs(args->x_llcoord) + fabs(args->x_upcoord)) / args->width;
  	args->ystep = (fabs(args->y_llcoord) + fabs(args->y_upcoord)) / args->height;
	num_threads = atoi(calc_params[7]);
	
	printf("Image width: %d\n", (int)args->width);
	printf("Image height: %d\n", (int)args->height);
	printf("Lower-left: (%.6f,%.6f)\n", args->x_llcoord, args->y_llcoord);
	printf("Upper-right: (%.6f,%.6f)\n", args->x_upcoord, args->y_upcoord);
	printf("Number of threads: %d\n", num_threads);
	printf("Output image name: %s\n", calc_params[0]);
	
  	//Create the image array
  	args->pixels = malloc((args->width * args->height) * 3);

  	//Begin pthread creation/initialization
  	pthread_t threads[num_threads];
  	pthread_attr_t attr;
  	pthread_attr_init (&attr);
  	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

  	//Find the space between each row used to calculate the set
  	y_chunk = (fabs(args->y_llcoord) + fabs(args->y_upcoord)) / num_threads;

  	/*Set up the thread to begin calculating the image*/
  	for (ii = 0; ii < num_threads; ii++)
  	{
    		args->x_start = args->x_llcoord;
    		args->y_start = args->y_llcoord;

		args->y_start += (ii * y_chunk);
		args->y_end = args->y_start + y_chunk;
		
		printf("Thread %d starts at y value %.6f\n", ii, args->y_start);
    		rc = pthread_create(&threads[ii], &attr,(void*)calc_mandelbrot_multi,(void*)args);
    		assert(rc == 0);
  	}

  	/*Join the threads back together*/
  	for (ii = 0; ii < num_threads; ii++)
  	{
    		rc = pthread_join (threads[ii], &status);
    		assert (rc == 0);
  	}

  	//Write the image to the file
  	write_file(calc_params[0],args->pixels, atoi(calc_params[2]), atoi(calc_params[1]));

  	//Free up the dangling pointers
	free(file_str);
  	free(args);
	fp = NULL;
  	return 0;
}


/*Multi-threaded helper function to calculate Mandelbrot Calculation*/
void
calc_mandelbrot_multi(void* args)
{
  //Initialize a local stack copy of the argument structure and copy it
  struct arg_struct multi_args;
	memcpy(&multi_args,args,sizeof(arg_struct));

  //Calculate the color for each pixel here
  calc_mandelbrot(multi_args.pixels,multi_args.x_start,multi_args.x_end,multi_args.y_llcoord,
                  multi_args.y_upcoord,multi_args.xstep,multi_args.ystep,multi_args.height,
                  multi_args.width);

  pthread_exit(NULL);
}
/* Unithreaded Mandelbrot calculation */
static void
calc_mandelbrot (uint8_t * pic, double xstart, double xend, double ystart,
                 double yend, double xstep, double ystep, uint32_t height,
                 uint32_t width)
{

  double x,y;
  int i,j;
  uint8_t r,g,b;
  double v;

  assert (pic != NULL);

  /* the main loop */
  x = xstart;
  y = ystart;

  /* for each row in the image */
  for (i=0; i<height; i++)
  {
    /* for each column within a row */
    for (j=0; j<width; j++)
    {
      /* calculate the RGB pixel values */
      calc_pixel (x, y, &r, &g, &b, &v);
      pic[(i * width + j) * 3 + 0] = b;
      pic[(i * width + j) * 3 + 1] = g;
      pic[(i * width + j) * 3 + 2] = r;

      /* move to the next x value in the row */
      x += xstep;
    }

    /* move to the next y value and start x back at the beginning */
    y += ystep;
    x = xstart;
  }
}

/* Calculate the RGB pixel values for a pair of x,y coordinates. */
static void
calc_pixel (double x, double y, uint8_t * red, uint8_t * green, uint8_t * blue,
            double * value)
{
  /* z is the real part, zi is the imaginary part of the complex number */
  double z = 0, zi = 0;
  double newz,newzi;    /* used for updating z and zi */
  double color;         /* pixel color */
  bool inset = true;    /* is the point in the Mandelbrot set? */
  int k;

  /* Iterate through the Mandelbrot calculation 500 times. If we ever exceed
   * the threshold, then break out of the loop; we know that point is not in
   * the set. */
  for (k=0; k < NUM_ITERATIONS; k++)
    {
      /* z^2 + (x+yi) = (a+bi)(a+bi) + (x+yi) = a^2 - b^2 + x + (2ab + y)i */
      newz = (z*z)-(zi*zi) + x; /* a^2 - b^2 + x */
      newzi = 2*z*zi + y;       /* (2ab + y)i */
      z = newz;
      zi = newzi;
      if(((z*z)+(zi*zi)) > 100)
        {
          inset = false;
          color = k;
          k = NUM_ITERATIONS;
        }
    }

  /* evaluate the magnitude of the distance of (z,zi) from (0,0) */
  *value = (z*z)+(zi*zi);

  if (inset)
    {
      /* points in the set are a blue-ish color */
      *blue = 230;
      *green = 50;
      *red = 70;
    }
  else
    {
      /* points out of the set are green-ish, lighter as they get closer */
      if (color < 50) color += 50;
      else if (color < 100) color += 55;
      else if (color < 350) color += 80;
      *blue = color / NUM_ITERATIONS * 180;
      *green = color / NUM_ITERATIONS * 255;
      *red = color / NUM_ITERATIONS * 120;
    }
}
