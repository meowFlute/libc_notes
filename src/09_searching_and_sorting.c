#include "09_searching_and_sorting.h"

#include <stdlib.h>
#include <stdio.h>

/* one static prototype per subsection */
static void comparison_functions(comparison_fn_t);
static void array_search_function(comparison_fn_t);
static void array_sort_function(comparison_fn_t);
static void hash_search_function(void);
static void tree_search_function(void);
int compare_doubles(const void *, const void *);

/* runnable for main */
void search_sort_run_demos(void)
{
    comparison_functions(compare_doubles);
    array_search_function(compare_doubles);
    array_sort_function(compare_doubles);
    hash_search_function();
    tree_search_function();
}

/* 9.1 -- Defining the Comparison Function
 * In order to do arbitrary comparisons on any object, we create a function
 * that:
 * - takes two void pointers as arguments
 * - casts those void pointers to the appropriate types 
 * - returns a comparison the same way strcmp does, that is:
 *      1) return negative if the first argument is "less" 
 *      2) return zero if they are equal
 *      3) return positive if the first argument is greater 
 *
 * stdlib.h defines:
 *      int comparison_fn_t (const void *, const void *)
 * as a name for the data type of comparison functions */
int compare_doubles(const void * a, const void * b)
{
    const double *da = (const double *) a;
    const double *db = (const double *) b;

    return (*da > *db) - (*da < *db);
}
/* used the comparison function pointer type defined in stdlib */
static void comparison_functions(comparison_fn_t compare_fn_in)
{
    printf( "printing address of 'comparison_fn_t' comparison function: %p\n",
            compare_fn_in);
}

/* 9.2 -- Array Search Function
 * 
 * Searching you can do one of two things:
 * 1) Use the linear methods provided by lfind and lsearch for unsorted arrays
 * 2) Use the binary search algorith "bsearch" function for an array that has 
 * been sorted by qsort using the same comparison function
 */
#define DOUBLE_ARRAY_LEN 10
static void array_search_function(comparison_fn_t compare_func)
{
    double d_arr[DOUBLE_ARRAY_LEN] = { 0 };
    double key = 0.0;
    printf("Random array to qsort =\n\t{ ");
    for (size_t i = 0; i < DOUBLE_ARRAY_LEN; i++)
    {
        d_arr[i] = drand48() * 100.0;
        if(i < (DOUBLE_ARRAY_LEN - 1))
            printf("%.5lf, ", d_arr[i]);
        else
            printf("%.5lf }\n",d_arr[i]);
    }
    key = d_arr[5];

    /* linear search methods */

    qsort(d_arr, DOUBLE_ARRAY_LEN, sizeof(double), compare_func);
    /* binary search method on a sorted array */
}

/* 9.3 -- Array Sort Function
 *
 * qsort provides an interface that allows you to use the "quick sort"
 * algorithm. The implementation of qsort in this library might not be an
 * in-place sort and might thereby use an extra amount of memory to store the
 * array.
 */
static void array_sort_function(comparison_fn_t compare_func)
{
    double d_arr[DOUBLE_ARRAY_LEN] = { 0 };
    /* here is an example of how to use qsort */
    printf("Random array to qsort =\n\t{ ");
    for (size_t i = 0; i < DOUBLE_ARRAY_LEN; i++)
    {
        d_arr[i] = drand48() * 100.0;
        if(i < (DOUBLE_ARRAY_LEN - 1))
            printf("%.5lf, ", d_arr[i]);
        else
            printf("%.5lf }\n",d_arr[i]);
    }

    qsort(d_arr, DOUBLE_ARRAY_LEN, sizeof(double), compare_func);
    printf("Sorted array =\n\t{ ");
    for (size_t i = 0; i < DOUBLE_ARRAY_LEN; i++)
    {
        if(i < (DOUBLE_ARRAY_LEN - 1))
            printf("%.5lf, ", d_arr[i]);
        else
            printf("%.5lf }\n",d_arr[i]);
    }
}

static void hash_search_function(void)
{

}

static void tree_search_function(void)
{

}
