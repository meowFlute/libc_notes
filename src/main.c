#include <stdio.h>
#include <stdlib.h>

/* notes files -- all have an associated .c */
#include "2_libc_error_reporting_notes.h"
#include "3_libc_virtual_memory_allocation_and_paging_notes.h"
#include "4_character_classification.h"

int main(int argc, char * argv[])
{
    /* I don't feel like using getopt or whatever, but maybe I'll add that to
     * future iterations of my init script */
    if(argc > 1)
    {
        printf("Don't pass me any arguments, just run '%s'\n", argv[0]);
        printf("USAGE: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }
   
    /* Chapter 3 -- memory management examples */
    get_memory_subsystem_info();
    virtual_memory_allocation_example();
    paging_example();

    /* Chapter 4 -- Character Classification Examples */
    char_classification_demo();
    char_case_conversion_demo();

    /* Chapter 2 -- error reporting should be run last because it will exit with
     * return value of EXIT_FAILURE before we reach the exit below */
    printf("Starting error reporting example:\n");
    error_reporting_example();
    /* we won't reach anything below this point, that's part of the demo */
    printf("Error reporting example complete.\n\n");
    exit(EXIT_SUCCESS);
}
