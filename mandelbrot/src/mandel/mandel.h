#ifndef __MANDELBROT_HEADER__
#define __MANDELBROT_HEADER__

#include <stdint.h>             /* C99 integer types */

/* struct to hold the values for each pixel in the Mandelbrot set calculations.
 * This structure will be used in queries to check that Mandelbrot set values
 * are correct for the multithreaded implementation */
typedef struct index_value
{
  uint32_t i;       /* the row in the image for this pixel */
  uint32_t j;       /* the column in the image for this pixel */
  double value;     /* the magnitude of the (z,zi) distance */
} index_value;

int run_mandelbrot (char *params, index_value ** array);

typedef struct arg_struct {
		 uint8_t *pixels; /* The pixles in the image */
		 double x_llcoord; /* The real x-value of the lower left corner */
		 double y_llcoord; /* The real y-value of the lower left corner */
		 double x_upcoord; /* The real x-value of the upper right corner */
		 double y_upcoord; /* The real y-value of the upper right corner */
		 double xstep; /* The distance between real-x values in the image */
		 double ystep; /* The distance between real-y values in the image */
		 double width; /* The width of the image */
		 double height; /* The height of the image */
		 double x_start; /* The starting x-value for the thread */
		 double x_end; /* The ending x-value for the thread */
		 double y_start; /* The starting y-value for the thread */
		 double y_end; /* The ending y-value for the thread */
} arg_struct;


#endif
