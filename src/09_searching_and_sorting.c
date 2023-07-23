#include "09_searching_and_sorting.h"

#include <stdlib.h>
#include <stdio.h>

/* one static prototype per subsection */
static void comparison_functions(comparison_fn_t);
static void array_search_function(void);
static void array_sort_function(void);
static void hash_search_function(void);
static void tree_search_function(void);
int compare_doubles(const void *, const void *);

/* runnable for main */
void search_sort_run_demos(void)
{
    comparison_functions(compare_doubles);
    array_search_function();
    array_sort_function();
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
static void comparison_functions(comparison_fn_t compare_fn_in)
{
    printf( "printing address of 'comparison_fn_t' comparison function: %p\n",
            compare_fn_in);
}

static void array_search_function(void)
{

}

static void array_sort_function(void)
{

}

static void hash_search_function(void)
{

}

static void tree_search_function(void)
{

}
