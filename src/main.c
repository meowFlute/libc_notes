#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

/* notes files -- all have an associated .c */
#include "02_error_reporting.h"
#include "03_virtual_memory_allocation.h"
#include "04_character_classification.h"
#include "25_program_arguments.h"

int main(int argc, char * argv[])
{
    int ret = parse_arguments_argp_demo(argc, argv);
    if(ret != EXIT_SUCCESS)
        error(EXIT_FAILURE, errno, "Argument Parsing Failure");

    /* Section 3 -- memory management demo */
    if(sections[3])
    {
        get_memory_subsystem_info();
        virtual_memory_allocation_demo();
        paging_demo();
    }

    /* Section 4 -- Character Classification Examples */
    if(sections[4])
    {
        char_classification_demo();
        char_case_conversion_demo();
        wchar_classification_demo();
        wchar_usage_demo();
        wchar_mapping_demo();
    }

    /* Section 2 -- error reporting should be run last because it will exit with
     * return value of EXIT_FAILURE before we reach the exit below */
    if(sections[2])
    {
        printf("Starting error reporting demo:\n");
        error_reporting_demo();
        /* we won't reach anything below this point, that's part of the demo */
        printf("Error reporting demo complete.\n\n");
    }
        
    exit(EXIT_SUCCESS);
}
