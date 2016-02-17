/* Unithreaded merge sort implementation */

#include <stdio.h>              /* standard I/O */
#include <stdbool.h>            /* C99 boolean types */
#include <stdint.h>             /* C99 integer types */
#include <string.h>             /* declares memcpy() */
#include <pthread.h>            /* pthread library  */
#include <stdlib.h>             /* malloc */
#include <assert.h>							/* Assert statements */
#include "msort.h"              /* merge sort interface header */


void msort_multi_wrapper(index_value array[], uint32_t start, uint32_t end,
            bool (*less)(index_value, index_value), uint32_t bsort,uint8_t current_depth,
						uint8_t parallel_depth);

void merge_sort (index_value array[], uint32_t start, uint32_t end,
                 bool (*less)(index_value, index_value), uint32_t bsort);
void merge (index_value array[], uint32_t start, uint32_t end,
            bool (*less)(index_value, index_value));
void bubble (index_value array[], uint32_t start, uint32_t end,
             bool (*less)(index_value, index_value));
void msort_multi(void *);

/* entry point to merge sort. Do not change the interface */
int
run_msort (char * params, index_value ** _array)
{
  	//Struct to hold the arguments for the multi-threading
  	ms_args *msort_params;

  	//File IO variables
  	char* calc_params[5];
  	char* calc_duplicate;
  	char *file_str = NULL;
  	uint32_t file_size;
  
  	//Loop control variables
  	uint32_t ii;
  	uint8_t aa;

  	//Variables retrieved from the file
  	uint8_t num_threads;
  	uint32_t lc;
  
  	//Assertion check for thread creation
  	uint32_t rc;

 	//Pthread attributes	
  	pthread_attr_t attr;
  	void* status;

  	//File parameter IO
  	FILE * fp;
  	fp = fopen(params,"r");
  	fseek(fp,0,SEEK_END);
  	file_size = ftell(fp);
  	rewind(fp);
  	file_str = malloc(file_size);
  	fgets(file_str,file_size,fp);

  	//Duplicate the string to keep original
  	calc_duplicate = strdup(file_str);

  	//Assign the parameters properly
  	for(ii = 0;ii < 5;ii++)
  	{
   		 calc_params[ii] = strsep(&calc_duplicate,",");
  	}

  	//Allocate memory for the parameters	
  	msort_params = (ms_args*)malloc(sizeof(ms_args));

	/*
	 * 0 - output file name
	 * 1 - input file name
	 * 2 - number of lines to read
	 * 3 - parallel depth
	 * 4 - number of threads to be used/times to be split
	 * 5 - bubble sort threshold
	 */

	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	// malloc array based off of calc_params	
	lc = atoi(calc_params[2]);
	fp = fopen(calc_params[1],"r");
	fseek(fp,0,SEEK_END);
	rewind(fp);
  	
	//Retrieve the unsorted array metadata
	msort_params->array = malloc(sizeof(index_value)*lc);	
	msort_params->start = 0;
  	msort_params->end = (atoi(calc_params[2]) - 1);
	msort_params->parallel_depth = atoi(calc_params[3]);
	msort_params->less = comp_coords;
	msort_params->bubble_threshold = atol(calc_params[4]);
	msort_params->cur_depth = 0;
	
	//Pthread information
	num_threads = msort_params->parallel_depth;
  	pthread_t threads[num_threads];

	//Begin tokenizing the data in the unsorted array file
  	char* token;

	//Loop tokenizing here
  	ii = 0;
  	while(fgets(file_str, 128, fp) != NULL)
	{
		//Get first token
    		token = strtok(file_str, ",");
    		aa=0;
    		while(token != NULL) //Loop until you run out of tokens
    		{
      			switch(aa) //Assign them to their proper location
      			{
        			case 0: 
            				msort_params->array[ii].i = atoi(token); 
            				break;
        			case 1: 
            				msort_params->array[ii].j = atoi(token); 
            				break;
        			case 2:
            				msort_params->array[ii].value = atof(token); 
            				break;
      			}
      			token = strtok(NULL, ",");
      			aa++;
    		}
    		ii++;
	}

	//Create pthreads and pass argument struct
  	for (ii = 0 ;ii < num_threads; ii++)
  	{
    		rc = pthread_create(&threads[ii],&attr,(void*) msort_multi,(void*)msort_params);
		assert(rc == 0);
  	}
	
	//Join pthreads back together
	for(ii = 0;ii < num_threads;ii++)
	{
		rc = pthread_join(threads[ii],&status);
    		assert(rc == 0);
	}

	//Open sorted array file
	fp = fopen(calc_params[0],"wb");
  	assert(msort_params->array != NULL);
 
	//Write the output appropriately
   	for(ii = 0;ii < lc;ii++)
  	{
    		fprintf(fp,"%d,%d,%f\n",msort_params->array[ii].i,msort_params->array[ii].j,
            	msort_params->array[ii].value);
  	}

	//Close the dangling pointers
  	fclose(fp);
  	fp = NULL;
	return 0;
}

/* unithreaded merge sort implementation */
void
merge_sort (index_value array[], uint32_t start, uint32_t end,
            bool (*less)(index_value, index_value), uint32_t bsort)
{
  /* calculate the index in the middle of the array */
  uint32_t middle = (end + start) / 2;
  uint32_t right_start = middle + 1;

  /* if the size has fallen below the threshold, switch to bubble sort */
  if ( (end - start) < bsort)
    {
      bubble (array, start, end, less);
      return;
    }


  /* split the array into a left and right half, and sort both */
  merge_sort (array, start, middle, less, bsort);
  merge_sort (array, right_start, end, less, bsort);

  /* merge the sorted arrays */
  merge (array, start, end, less);

//  pthread_exit(NULL);
}

/* Multi-Threaded Implementation of merge-sort*/
void msort_multi(void * _args)
{
	ms_args *msort_local;
	msort_local = (ms_args*)_args;

	msort_multi_wrapper(msort_local->array,msort_local->start,msort_local->end,
				msort_local->less, msort_local->bubble_threshold,
				msort_local->cur_depth, msort_local->parallel_depth);
}

void msort_multi_wrapper(index_value array[], uint32_t start, uint32_t end,
            bool (*less)(index_value, index_value), uint32_t bsort,uint8_t current_depth,
						uint8_t parallel_depth)
{
	//Calculate the middle of the array
	uint32_t middle = (end + start) / 2;
  	uint32_t right_start = middle + 1;

	//If the recursion is enough, hit the base case
	if(current_depth >= parallel_depth)
	{
		merge_sort(array,start,end,less,bsort);
	}
	else
	{
		msort_multi_wrapper(array,start,middle,less,bsort,current_depth + 1,parallel_depth);
		msort_multi_wrapper(array,right_start,end,less,bsort,current_depth + 1,parallel_depth);
      		merge(array, start, end, less);
	}
}

#define MAX_TEMP_ARRAY 128
/* routine to merge two sorted arrays. You should not change this. */
void
merge (index_value array[], uint32_t start, uint32_t end,
       bool (*less)(index_value, index_value))
{
  index_value temp[MAX_TEMP_ARRAY];
  uint32_t temp_index, i;
  uint32_t right_index = ((end + start) / 2) + 1;
  uint32_t left_index = start;

  /* start at the beginning of the left and right arrays */
  while (right_index <= end && left_index < right_index)
    {
      /* if array[right] < array[left], swap them */
      if (less (array[right_index], array[left_index]))
        {
          /* Get several items from the right that are bigger than left. This
           * provides a minor performance improvement. */
          i = 0;
          do
            {
              i++;
            } while (less (array[right_index+i], array[left_index]) &&
                     i < MAX_TEMP_ARRAY && (right_index + i) <= end);

          /* copy up to 128 items from the right into a temp array */
          memcpy (temp, &array[right_index], i * sizeof (index_value));

          /* shift the left array to the right */
          for (temp_index = right_index; temp_index > left_index;
               temp_index--)
            array[temp_index + i - 1] = array[temp_index - 1];

          /* now copy the temp items into the left */
          memcpy (&array[left_index], temp, i * sizeof (index_value));
          right_index += i;
        }
      left_index++;
    }
}
#undef MAX_TEMP_ARRAY

/* bubble sort implementation. This is just basic bubble sort and shouldn't
 * need modified. */
void
bubble (index_value array[], uint32_t start, uint32_t end,
        bool (*less)(index_value, index_value))
{
  uint32_t i, j;
  index_value temp;

  for (i = end; i > start; i--)
    {
      for (j = start; j < i; j++)
        {
          if (less (array[j+1], array[j]))
            {
              temp = array[j];
              array[j] = array[j+1];
              array[j+1] = temp;
            }
        }
    }
}
  
/* comparison function to compare the (i,j) coordinates of two pixels */
bool
comp_coords (index_value a, index_value b)
{
  if (a.i < b.i) return true;
  if ( (a.i == b.i) && (a.j < b.j) ) return true;
  return false;
}

