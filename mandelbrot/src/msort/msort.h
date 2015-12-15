#ifndef __MSORT_HEADER__
#define __MSORT_HEADER__

#include <stdint.h>             /* C99 integer types */
#include <stdbool.h>            /* C99 boolean types */
#include "../mandel/mandel.h"   /* index_value */

int run_msort (char *params, index_value ** array);
void * merge_sort_thread (void * _input);
bool comp_index (index_value a, index_value b);
bool comp_coords (index_value a, index_value b);

/* structure that can encapsulate all arguments needed for merge sort. Notice
 * the definition of less, which is a function pointer */
typedef struct ms_args
{
  uint8_t cur_depth;
  index_value *array;
  uint32_t start;
  uint32_t end;
  uint8_t parallel_depth;
  bool (*less)(index_value, index_value);
  uint32_t bubble_threshold;
} ms_args;

#endif
