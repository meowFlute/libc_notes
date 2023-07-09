/* Unconstrained Allocation Demos:
 *   'void *malloc (size_t SIZE)’
 *        Allocate a block of SIZE bytes.  *Note Basic Allocation::.
 *   
 *   ‘void free (void *ADDR)’
 *        Free a block previously allocated by ‘malloc’.  *Note Freeing after
 *        Malloc::.
 *   
 *   ‘void *realloc (void *ADDR, size_t SIZE)’
 *        Make a block previously allocated by ‘malloc’ larger or smaller,
 *        possibly by copying it to a new location.  *Note Changing Block
 *        Size::.
 *   
 *   ‘void *reallocarray (void *PTR, size_t NMEMB, size_t SIZE)’
 *        Change the size of a block previously allocated by ‘malloc’ to
 *        ‘NMEMB * SIZE’ bytes as with ‘realloc’.  *Note Changing Block
 *        Size::.
 *   
 *   ‘void *calloc (size_t COUNT, size_t ELTSIZE)’
 *        Allocate a block of COUNT * ELTSIZE bytes using ‘malloc’, and set
 *        its contents to zero.  *Note Allocating Cleared Space::.
 *   
 *   ‘void *valloc (size_t SIZE)’
 *        Allocate a block of SIZE bytes, starting on a page boundary.  *Note
 *        Aligned Memory Blocks::.
 *   
 *   ‘void *aligned_alloc (size_t SIZE, size_t ALIGNMENT)’
 *        Allocate a block of SIZE bytes, starting on an address that is a
 *        multiple of ALIGNMENT.  *Note Aligned Memory Blocks::.
 *   
 *   ‘int posix_memalign (void **MEMPTR, size_t ALIGNMENT, size_t SIZE)’
 *        Allocate a block of SIZE bytes, starting on an address that is a
 *        multiple of ALIGNMENT.  *Note Aligned Memory Blocks::.
 *   
 *   ‘void *memalign (size_t SIZE, size_t BOUNDARY)’
 *        Allocate a block of SIZE bytes, starting on an address that is a
 *        multiple of BOUNDARY.  *Note Aligned Memory Blocks::.
 *   
 *   ‘int mallopt (int PARAM, int VALUE)’
 *        Adjust a tunable parameter.  *Note Malloc Tunable Parameters::.
 *   
 *   ‘int mcheck (void (*ABORTFN) (void))’
 *        Tell ‘malloc’ to perform occasional consistency checks on
 *        dynamically allocated memory, and to call ABORTFN when an
 *        inconsistency is found.  *Note Heap Consistency Checking::.
 *   
 *   ‘struct mallinfo2 mallinfo2 (void)’
 *        Return information about the current dynamic memory usage.  *Note
 *        Statistics of Malloc::.
 * */

#include "3_virtual_memory_allocation.h"
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, calloc, free */
#include <malloc.h> /* mallopt, mallinfo2, malloc tunable parameters */
#include <mcheck.h> /* mcheck heap consistency checking, mtrace tracing */
#include <errno.h>  /* errno */
#include <error.h>  /* error */
#include <string.h> /* strcpy, strlen */
#include <unistd.h> /* sysconf */

typedef struct _llnode {
    int numeric_data;
    char * string_data;
    struct _llnode * next;
} llnode;

static llnode * new_llnode(int numeric_data, char * string_data)
{
    if(string_data != NULL)
    {
        /* for strings you have to allocate one more byte for the null
         * termination character */
        /* NOTE: the libc manual says in 3.2.3.1 "You can store the result of
         * 'malloc' into any pointer variable without a cast, because ISO C
         * automatically converts the type 'void *' to another type of pointer
         * when necessary. However, a cast is necessary if the type is needed
         * but not specified by context" */
        char * new_string_data = malloc(strlen(string_data) + 1);
        if(new_string_data == NULL)
        {
            int errno_tmp = errno;
            error(  EXIT_FAILURE, 
                    errno_tmp, 
                    "malloc of new_string_data failed. int = %d, string = %s\n",
                    numeric_data,
                    new_string_data);
        }

        strcpy(new_string_data, string_data);

        /* sizeof is a good utility for most things */
        llnode * new_node = malloc(sizeof(llnode));
        /* we need to check that malloc returned correctly, it will return a
         * null pointer otherwise and set errno */
        if(new_node == NULL)
        {
            int errno_tmp = errno;
            error(  EXIT_FAILURE, 
                    errno_tmp, 
                    "malloc of llnode failed. int = %d, string = %s\n",
                    numeric_data,
                    new_string_data);
        }

        /* here is where we initialize the members of the new struct */
        new_node->numeric_data = numeric_data;
        new_node->string_data = new_string_data;
        new_node->next = NULL;

        return new_node;
    }
    else
    {
        /* if the string passed in isn't usable we return a null pointer and
         * set errno to invalid argument */
        errno = EINVAL;
        return NULL;
    }
}

static void free_node(llnode * node_to_free)
{
    /* you need to free the data before you can free the containter */
    free(node_to_free->string_data);
    free(node_to_free);
    /* free doesn't return anything or set errno, so there is no feedback to
     * give */
    return;
}

static void free_entire_list(llnode * head_node)
{
    llnode * current_node = head_node;
    llnode * next_node = head_node->next;
    free_node(current_node);
    while(next_node != NULL)
    {
        current_node = next_node;
        next_node = current_node->next;
        free_node(current_node);
    }
}

static void print_entire_list(llnode * head_node)
{
    llnode * current_node = head_node;
    printf("Node number: %d, string_data = %s\n", 
                current_node->numeric_data,
                current_node->string_data);
    while(current_node->next != NULL)
    {
        current_node = current_node->next;
        printf("Node number: %d, string_data = %s\n", 
                current_node->numeric_data,
                current_node->string_data);
    }

    return;
}

int change_node_string(llnode * node, char * string_data)
{
    if(string_data != NULL)
    {
        /* here we do not malloc a new copy and free the old, we use realloc
         * instead as a perfect example of when you'd use realloc or
         * reallocarray */
        node->string_data = realloc(node->string_data, strlen(string_data)+1 );
        //reallocarray would work better here if: 
        //  - we had a multiplication of bytes times length
        //  (1*strlen(string_data) doesn't really count in my mind) 
        //  - we were concerned about an overflow on the size field, because it
        //  handles it implicitly

        /* realloc and reallocarray return NULL and set errno on error */
        if(node->string_data == NULL)
        {
            int errno_tmp = errno; //save off errno before doing any printf
            error(EXIT_FAILURE, 
                    errno_tmp,
                    "realloc of string_data failed, errno = %d",
                    errno_tmp);
        }

        /* otherwise we successfully realloced and set the pointer */
        strcpy(node->string_data, string_data);

        return 0;        
    }
    else
    {
        /* Error return, set errno to invalid argument and return -1 */
        errno = EINVAL;
        return -1;
    }
}

void print_memory_statistics(struct mallinfo2 malloc_info)
{
    printf("Total size of memory allocated using sbrk: %zu\n\t", 
            malloc_info.arena);
    printf("Number of chunks not in use: %zu\n\t", 
            malloc_info.ordblks);
    printf("Total size of memory occupied by chunks handed out by malloc:" 
           " %zu\n\t", 
            malloc_info.uordblks);
    printf("Number of chunks occupied by free: %zu\n\t", 
            malloc_info.fordblks);
    printf("Size of the top-most releasable chunk: %zu\n", 
            malloc_info.keepcost);

    return;
}

void virtual_memory_allocation_demo(void)
{
    /* auto is a form of memory management that blows this away when it leaves 
     * scope, but it also happens automatically so it is pointless */
    char unique_string[80]; 
    int i = 1;

    /* we'll trace memory allocation for this function only
     * to make it work you have to do the following:
     * 1) preload the debugging library libc_malloc_debug using the LD_PRELOAD
     *      environment variable
     * 2) export the environment variable MALLOC_TRACE to be a valid file which
     *      can be opened for writing 
     *
     * In practice this would look like the following from the terminal:
     *  
     *  $ LD_PRELOAD=/lib/x86_64-linux-gnu/libc_malloc_debug.so \
     *  MALLOC_TRACE=$HOME/libc_notes/malloc.trace ./libc_notes
     *  $ mtrace malloc.trace
     *  No memory leaks.
     *  $
     * */
    mtrace();

    printf("Memory statistics prior to mallocs:\n\t");
    print_memory_statistics(mallinfo2());

    sprintf(unique_string, "This is my head node: %d", i);
    llnode * list = new_llnode(i++, unique_string);
    if(list == NULL)
        error(EXIT_FAILURE, errno, "new_llnode failed loc 1");

    printf("Memory statistics after first to malloc:\n\t");
    print_memory_statistics(mallinfo2());

    llnode * current_node = list;
    while(i <= 5)
    {
        sprintf(unique_string, "This is a chained node: %d", i);
        current_node->next = new_llnode(i++, unique_string);
        if(current_node->next == NULL)
            error(EXIT_FAILURE, errno, "new_llnode failed loc 2");
        current_node = current_node->next;
    }

    printf("Memory statistic after all mallocs:\n\t");
    print_memory_statistics(mallinfo2());

    /* demo the malloc success */
    printf("Printing list after init mallocs:\n");
    print_entire_list(list);

    /* realloc a string and reprint to demo realloc */
    printf("Changing a string at node 3 and reprinting\n");
    int result = change_node_string(list->next->next, 
                                        "This string has been changed");
    if(result == -1)
        error(0, errno, "passed a null string to change_node_string");
    print_entire_list(list);

    printf("Memory statistic after realloc:\n\t");
    print_memory_statistics(mallinfo2());
    
    /* calloc example */
    printf("Creating a node via calloc:\n");
    llnode * callocd_node = calloc(1, sizeof(llnode));
    if(callocd_node == NULL)
        error(EXIT_FAILURE, errno, "Error occurred during calloc");
    printf("\tNode contents:\n\tint = %d\n\tchar* = %p\n\tnode* = %p\n",
            callocd_node->numeric_data,
            callocd_node->string_data,
            callocd_node->next);

    printf("Memory statistic after calloc:\n\t");
    print_memory_statistics(mallinfo2());
 

    /* you can use the same free for realloc or callocd memory */
    free_node(callocd_node);

    printf("Memory statistic after freeing calloc:\n\t");
    print_memory_statistics(mallinfo2());

    callocd_node = NULL;

    free_entire_list(list);

    printf("Memory statistic after freeing full list:\n\t");
    print_memory_statistics(mallinfo2());

    /* This deinstalls the handlers for malloc etc. loaded by mtrace() above
     * and then closes the protocol file set in MALLOC_TRACE. After this the
     * program runs at full speed once more 
     *
     * generally speaking it is better to just not call muntrace and call
     * mtrace() first thing on entry to main, but we'll do it this way for
     * demonstration.
     *
     * The reason for this is because other library functions will call malloc
     * and such as needed and you might cut them off before they can free and
     * be looking at a red herring instead of a bug. */
    muntrace();

    list = NULL;
    return;
}

/* not all page sizes are necessarily equal, the only thing that you can really
 * assum about them is that they come in powers of 2. There are situations
 * where it is useful to know the page size (e.g. mmap) */
void get_memory_subsystem_info(void)
{
    // int page_size = getpagesize(); // apparently this is old and not very
    // portable
    /* Here is how you query page size */
    long int page_size = sysconf(_SC_PAGESIZE);
    /* My machine might also tell me the total number of pages the system has
     * (not necessarily all the memory available) */
    long int phy_pages = sysconf(_SC_PHYS_PAGES);
    /* The amount of memory the application can use without hindering any other
     * process (assuming no other process increases its memory usage */
    long int avphy_pages = sysconf(_SC_AVPHYS_PAGES);

    printf("Page Size: %ld kilobytes\n", page_size / 1024);
    printf("Total Number of Available Pages: %ld pages\n\t%ld kilobytes\n", 
            phy_pages, phy_pages * page_size / 1024);
    printf("Total amount usable by application: %ld pages\n\t%ld kilobytes\n", 
            avphy_pages, avphy_pages * page_size / 1024);

    return;
}

void paging_demo(void)
{
    /* this is kind of a cop-out but I decided that I'm not going to pursue the
     * other memory concepts for now for the reason that I can't even think of
     * how I would realistically use them! This includes the following:
     * - obstacks
     * - pages
     * - page permissions
     * - locking pages
     *
     * I suspect that if I do ever use pages it will be for permissions and/or
     * locking of pages (e.g. setting a page to be executable) but I'll just
     * stow that away in my personal memory (get it?) for later */
    return;
}
