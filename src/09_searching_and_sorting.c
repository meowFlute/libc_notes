#include "09_searching_and_sorting.h"

#include <stdlib.h> /* qsort, bsearch */
#include <stdio.h>  /* printf */
#include <search.h> /* lsearch, lfind, hash tables, trees */
#include <error.h>  /* error */
#include <errno.h>  /* errno */
#include <string.h> /* memcpy */

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
    printf("\t======================\n");
    printf("\t===== CHAPTER 9 ======\n");
    printf("\t======================\n");
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
    printf("\t========================\n");
    printf("\t===== Section 9.1 ======\n");
    printf("\t========================\n");
    printf( "printing address of 'comparison_fn_t' comparison function: %p\n",
            compare_fn_in);
    printf("\n");
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
    printf("\t========================\n");
    printf("\t===== Section 9.2 ======\n");
    printf("\t========================\n");
    size_t i;
    double d_arr[DOUBLE_ARRAY_LEN] = { 0 };
    double key = 0.0;
    printf("Random array =\n\t{ ");
    for (i = 0; i < DOUBLE_ARRAY_LEN; i++)
    {
        d_arr[i] = drand48();
        if(i < (DOUBLE_ARRAY_LEN - 1))
            printf("%.5lf, ", d_arr[i]);
        else
            printf("%.5lf }\n",d_arr[i]);
    }
    key = d_arr[5];

    /* linear search methods */
    /* only use these when the array isn't sorted */
    /* lfind does exactly what you'd expect linearly */
    size_t nmemb = DOUBLE_ARRAY_LEN;

    double * val = lfind(&key, d_arr, &nmemb, sizeof(double), compare_func);
    if(val == NULL)
    {
        printf("value \"%lf\" not found in array\n", key);
    }
    else
    {
        size_t val_index = val - d_arr;
        printf( "value \"%lf\" found at address %p:\n"
                "\td_arr[%zu]\n"
                "\t%zu bytes from d_arr at %p\n",
                key, val, val_index, val_index * sizeof(double), d_arr);
    }
    
    /* lsearch does the same thing, BUT IF IT DOESN'T FIND IT ADDS IT, you have
     * to make sure that there is enough memory */
    double * d_arr_w_extra = malloc(sizeof(double) * (DOUBLE_ARRAY_LEN + 1));
    double key10 = 10.0;
    double * val2 = lsearch(&key10, d_arr_w_extra, &nmemb, sizeof(double), 
            compare_func);
    size_t val2_index = val2 - d_arr_w_extra;
    printf( "value \"%lf\" added at address %p:\n"
            "\tnmemb = %zu\n"
            "\t%zu bytes from d_arr_extra at %p\n",
            key10, val2, nmemb, val2_index * sizeof(double), d_arr_w_extra); 


    qsort(d_arr, DOUBLE_ARRAY_LEN, sizeof(double), compare_func);
    /* binary search method on a sorted array */
    printf( "array sorted using qsort:\n"
            "\t{");
    for(i = 0; i < DOUBLE_ARRAY_LEN; i++)
    {
        if(i < (DOUBLE_ARRAY_LEN - 1))
            printf("%.5lf, ", d_arr[i]);
        else
            printf("%.5lf }\n",d_arr[i]);
    }
    double * val3 = bsearch(&key, d_arr, DOUBLE_ARRAY_LEN, sizeof(double), compare_func);
    if(val3 == NULL)
    {
        printf("Value %lf not found in array\n", key);
    }
    else
    {
        size_t index3 = val3 - d_arr;
        printf( "value \"%lf\" found at address %p:\n"
                "\td_arr[%zu]\n"
                "\t%zu bytes from d_arr at %p\n",
                key, val3, index3, index3 * sizeof(double), d_arr);
    }
    printf("\n");
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
    printf("\t========================\n");
    printf("\t===== Section 9.3 ======\n");
    printf("\t========================\n");
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

    printf("\n");
}

/* 9.5 -- Hash Tables
 * 
 * search.h contains methods to create, destroy, and use hash tables out of the
 * box.
 *
 * DICTIONARIES!!! key-value pairs! Whoop whoop
 *
 * "int hcreate(size_t NEL)" creates a table of NEL elements, which cannot be
 * grown, only destroyed, so make sure it is surely large enough for the
 * intended purpose. Also, hashing tables work inefficiently if they are more
 * than 80% full, so make it bigger than it needs to be. For reference, one can
 * check out Knuth's "The Art of Computer Porgramming, Part 3: Searching and
 * Sorting" for more information... Or so I'm told.
 *
 * Another weak point of hcreate is there can be AT MOST 1 hashing table used through the
 * whole program. It also uses static memory (not reentrant) so a gnu extension
 * hcreate_r is provided to make arbitrarily many hash tables, so I think I'll
 * typically default to using that instead. hdestroy_r is the reentrant destroy
 * function (as opposed to hdestroy).
 *
 * search returns entries
 * ENTRY is a typedef of "struct entry" which contains two member fields:
 *      1) char *key 
 *          must be a pointer to a null-terminated string
 *      2) void *data 
 *          a generic pointer for use by the application (to anything you want)
 * 
 * ENTRY * hsearch(ENTRY ITEM, ACTION ACTION)
 *
 * if using hsearch, you simply pass an ENTRY with either FIND or ENTER (from
 * the enum ACTION). If ENTRY is found a pointer to it is returned, otherwise
 * if it is not found and FIND was the ACTION a NULL pointer is returned. If
 * ENTRY is not found and ENTER was the ACTION then a pointer to a new ENTRY
 * that was initialized using ITEM is returned.
 * --
 * For the reentrant versions it is very similar except that you pass a struct
 * hsearch_data *HTAB in, which is initialized by 
 *      int hcreate_r(size_t NEL, struct hsearch_data *HTAB)
 * HTAB must be initialized with zero before calling create!
 *
 * return value is zero if there is a problem
 * --
 * hdestroy_r( struct hsearch_data *HTAB )
 *
 * is used to clean things up, BUT it is the program's responsibility to clean
 * up strings used in the element's table
 * --
 * int hsearch_r (ENTRY ITEM, ACTION ACTION, ENTRY **RETVAL, 
 *                  struct hsearch_data *HTAB)
 * 
 * RETVAL is used here so we can get an int status and set errno, which will
 * either be ENOMEM for when the table is filled, ESRCH when the parameter is
 * FIND and no element is found.
 */
static void hash_search_function(void)
{
    struct critter
    {
         const char *name;
         const char *species;
    };
    struct critter muppets[] =
    {
        {"Kermit",              "frog"},
        {"Piggy",               "pig"},
        {"Gonzo",               "whatever"},
        {"Fozzie",              "bear"},
        {"Sam",                 "eagle"},
        {"Robin",               "frog"},
        {"Animal",              "animal"},
        {"Camilla",             "chicken"},
        {"Sweetums",            "monster"},
        {"Dr. Strangepork",     "pig"},
        {"Link Hogthrob",       "pig"},
        {"Zoot",                "human"},
        {"Dr. Bunsen Honeydew", "human"},
        {"Beaker",              "human"},
        {"Swedish Chef",        "human"},
        {0, 0}
    };

    printf("\t========================\n");
    printf("\t===== Section 9.5 ======\n");
    printf("\t========================\n");
    
    size_t i;
    size_t num_elements = 50;
    ENTRY item;
    ENTRY * retval = NULL; 

    /* htab creation, with initialization to zero */
    /* could also use calloc, potato potato at this point */
    struct hsearch_data *htab = calloc(1, sizeof(struct hsearch_data));
    if(!hcreate_r(num_elements, htab))
        error(EXIT_FAILURE, errno, "hcreate_r failed");
    printf("\nhash table 1 created\n");

    /* Add the muppets with their names as keys */
    for(i = 0; muppets[i].name; i++)
    {
        /* initialize the item data to pass by value */
        item.key = (char *)muppets[i].name; // these are pointers, so if I 
                                            // modify that data later things
                                            // get weird I think
        item.data = (void *)(muppets[i].species);
        if(!hsearch_r(item, ENTER, &retval, htab))
            error(EXIT_FAILURE, errno, "hsearch_r(ENTER) failed");
        
        printf( "\tAdded key=\"%s\", value=\"%s\" to hash table\n",
                item.key, (char *)(item.data));
    }
    
    /* find Fozzie */
    printf("Looking up Fozzie\n");
    item.key = "Fozzie";
    item.data = NULL;
    if(!hsearch_r(item, FIND, &retval, htab))
        error(EXIT_FAILURE, errno, "hsearch_r(FIND) failed");

    printf( "Search results:\n"
            "\tkey=\"%s\"\n"
            "\tvalue=\"%s\"\n",
            retval->key, (char *)(retval->data));

    /* clean up */
    printf("destroying hash table\n");
    hdestroy_r(htab);
    printf("\n");
}

/* These functions implement a tree to access data with a mean access time
 * proportional to the logarithm of the number of elements, and the GNU C
 * Library implementation guarantees this bound is never exceeded even for
 * input data which would give binary trees issues.
 *
 * They also can be used with arbitrary data and carry no restrictions that the
 * hash tables did with their keys needing to be null-terminated strings
 *
 * Since they're not using strings as keys, you must specify a comparison
 * function (still just comparison_fn_t like previous sections of this chapter)
 *
 * Since these functions operate on void pointers, they do not know the size of
 * the containers they're looking at. This means they can't possibly copy them,
 * and the data added to the tree is added by pointer reference only. This
 * means memory management of the data pointed to by the tree nodes is done
 * externally to the tree itself and that data must remain available for the
 * lifetime of the tree.
 *
 * You can apply a function to the tree at every node as long as that function
 * has type
 *
 *      void __action_fn_t (const void *nodep, VISIT value, int level)
 *
 * where: 
 *  -   nodep is the data value of the current node, 
 *  -   visit is an enum indicating what type of node it is and when the 
 *      function was called, and 
 *  -   level is depth in the tree (root being 0, its children being 1, etc)
 *
 * If you want to destroy the whole tree you can define a function that frees
 * the data of type
 *
 *      void __free_fn_t ( void *nodep)
 * 
 * that does the freeing of elements. Even if you don't want it to free
 * elements, you still need to call something that does nothing
 */
void print_node_fn_info(const void *nodep, VISIT value, int level)
{
    char * visit_value = NULL;
    double data = **(double **)nodep;   
    /* convert VISIT value enum to string */
    switch (value)
    {
        case preorder :
            visit_value = "preorder";
            break;
        case postorder :
            visit_value = "postorder";
            break;
        case endorder :
            visit_value = "endorder";
            break;
        case leaf :
            visit_value = "leaf";
            break;
        default:
            visit_value = "unknown (default)";
            break;
    }

    printf( "Printing Node Info:\n"
            "\tconst void *nodep = %p\n"
            "\tdata in nodep = %lf\n"
            "\tVISIT value = %s\n"
            "\tlevel = %d\n",
            nodep,
            data,
            visit_value,
            level);
}
void print_free_elements(void * nodep)
{
    printf( "\tfunction called to free %lf\n",
            *((double *)nodep));
}
static void tree_search_function(void)
{
    printf("\t========================\n");
    printf("\t===== Section 9.6 ======\n");
    printf("\t========================\n");

    /* since we're using void pointers, you can start with a null'd out void
     * pointer for the root -- no tricky initialization sequence */
    void * root = NULL;
    void * cur_node = NULL;
#define NUM_DUBS 10UL
    double arr[NUM_DUBS] = { 1.0, 1.5, 2.5, 5.5, 10.3, 20.6, 32.4, 98.2, 100.2, 0.0 };
    double key;
    printf("pointer to arr for reference:\n\t%p", arr);

    /* tsearch adds a new node pointing to KEY if it not found, tfind does not */
    printf("\nusing tsearch to add nodes\n");
    for(size_t i = 0; i < NUM_DUBS; i++)
    {
        printf("\tadding %lf\n", arr[i]);
        cur_node = tsearch( (void *)(&arr[i]), &root, compare_doubles );
        if(cur_node == NULL)
            error(EXIT_FAILURE, errno, "tsearch out of memory");
    }
    
    printf("using tfind to search for two nodes\n");
    
    key = 2.5;
    cur_node = tfind(&key, &root, compare_doubles);
    if(cur_node == NULL)
        printf("\tkey %lf not found\n", key);
    else
        printf("\tkey %lf found\n", key);
    
    key = 2.7;
    cur_node = tfind(&key, &root, compare_doubles);
    if(cur_node == NULL)
        printf("\tkey %lf not found\n", key);
    else
        printf("\tkey %lf found\n", key);

    /* walk the tree applying a function at every leaf once and at every other
     * node three times based on timing */
    /* I wanted to do this before and after a delete to see how it changed the
     * topology */
    printf("walking the tree\n");
    twalk(root, print_node_fn_info);

    /* tdelete deletes a node matching KEY, something the hash tables couldn't
     * do */
    printf("deleting a single key %lf\n", arr[5]);
    key = arr[5];
    cur_node = tdelete(&key, &root, compare_doubles);
    if(cur_node == NULL)
        printf("\tkey %lf not found\n", key);
    else
        printf( "\tkey %lf deleted, parent was %lf\n", 
                key, 
                *((double *)cur_node));

    /* walk the tree applying a function at every leaf once and at every other
     * node three times based on timing */
    printf("walking the tree\n");
    twalk(root, print_node_fn_info);

    /* tdestroy blows away the entire tree */
    printf("calling tdestory to go scorched earth on this tree\n");
    tdestroy(root, print_free_elements);

    printf("\n");
}
