#include "25_program_arguments.h"
#include <argp.h>       /* argp functions */
#include <stdbool.h>    /* false */
#include <stdlib.h>     /* EXIT_SUCCESS */
#include <string.h>     /* strtok */

_Bool sections[39] = {false,};

/* argp globals */
const char * argp_program_version = "libc_notes version 5.9.0";
const char * argp_program_bug_address = "<mscottchristensen@gmail.com>";
const char doc[] = "libc notes -- a runnable set of examples subdivided by "
                    "sections of 'info libc' (the GNU libc Reference Manual)";

static error_t custom_parser(int key, char * arg, struct argp_state * state);

static struct argp_option options[] = {
        {"sections", 's', "CSV_SECTIONS", 0, 
            "comma-separated (no spaces) integers representing section numbers." 
            "e.g. 01,05,23"},
        { 0 }
    };

static struct argp parser = { options, custom_parser, 0, doc }; 

static error_t custom_parser(int key, char * arg, struct argp_state * state)
{
    /* --sections=01,04,23 */
    if(key == 's')
    {
        /* on the first call to strtok, you pass the delimited string */
        char * ptr = strtok(arg, ",");
        if(ptr == NULL)
            argp_error(state, "argument required for --sections");
        while(ptr != NULL)
        {
            int sub_arg = atoi(ptr);
            if(sub_arg > 0)
            {
                sections[sub_arg] = true;
            }
            else /* a return of 0 means an error */
            {
                argp_error(state, "Do not pass 0 as a section.");
            }
            ptr = strtok(NULL, ",");
        }
    }

    return 0;
}

/* This is taken from 25.3 "Parsing Program Options with Argp */
int parse_arguments_argp_demo(int argc, char * argv[])
{
    argp_parse(&parser, argc, argv, 0, 0, 0);

    return EXIT_SUCCESS;
}
