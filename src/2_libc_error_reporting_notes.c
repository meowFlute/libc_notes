#include "2_libc_error_reporting_notes.h"

/* in this example we intend to demonstrate the error reporting provided by
 * libc. You can divide that into the following groups:
 * 1) The errno info group
 * - volatile int errno                     (found in errno.h)
 *   ^ enumerated values to this guy are also found in errno.h
 *      ^ kind of... see note: #defines below code section
 * - strerrorname_np(int ERRNUM)            (found in string.h)
 * - strerrordesc_np(int ERRNUM)            (found in string.h)
 *
 * 2) The standard error reporting to a terminal group
 * - char * strerror(int ERRNUM)            (found in string.h)
 * - char * strerror_r(int ERRNUM)          (found in string.h)
 *   ^ these two are the same, _r goes in multi-threaded aplications
 * - void perror(const char *MESSAGE)       (found in stdio.h)
 * 
 * 3) The program information group
 * - char * program_invocation_name         (found in errno.h)
 * - char * program_invocation_short_name   (found in errno.h)
 *
 * 4) The GNU coding standard error assistants
 * - void error (int STATUS, int ERRNUM, const char *FORMAT, ...)
                                            (found in error.h)
 * - void error_at_line (int STATUS, int ERRNUM, const char
          *FNAME, unsigned int LINENO, const char *FORMAT, ...)
                                            (found in error,h)
 * -  void (*error_print_progname) (void)      (function pointer)
 * -  unsigned int error_message_count         (incremented by error calls)
 * -  int error_one_per_line                   (non-zero only tracks on per line)
 */
#include <errno.h>      /* errno */
#include <string.h>     /* strerror */
#include <stdio.h>      /* perror */
#include <error.h>      /* error */
#include <stdlib.h>     /* EXIT_FAILURE */

void error_reporting_example(void)
{
    /* to generate an error, I'll pass fopen a bad flag */
    error_message_count = 0;
    FILE * file = fopen("some_file", "bad_flags");
    if(file == NULL)
    {
        /* you need to save errno before you call anything else */
        int errno_saved = errno;
        printf("%s: fopen failed with errno = %d\n\t\tError Name: %s\n\t\tError Description: %s\n\t\tError: %s\n", 
                program_invocation_short_name,
                errno_saved, 
                strerrorname_np(errno_saved),
                strerrordesc_np(errno_saved),
                strerror(errno_saved)); /* seems like strerror is strerrordesc_np, basically */

        /* perror puts whatever string you pass it in front of the error */
        perror(program_invocation_name);
        
        /*  error and error_at_line */
        /*  0 in the STATUS field continues program execution */
        /*  non-zero gets passed to exit(STATUS) */
        error_at_line(0, errno, program_invocation_short_name, 41U, "fopen error found");
        error(EXIT_FAILURE, errno, "%u errors found", error_message_count); 
    }
}

/* note: #defines
 * because the errno.h header is nested in several files, just looking at it
 * could be a pain in the butt. On my chromebook running xubuntu I ran the
 * following per a stack overflow idea to get a sorted list of the relevant
 * codes that uses the c preprocessor
 *
 * $ cpp -dM /usr/include/errno.h | grep '#define E' | sort -n -k 3
 * #define EDEADLOCK EDEADLK
 * #define ENOTSUP EOPNOTSUPP
 * #define EWOULDBLOCK EAGAIN
 * #define EPERM 1
 * #define ENOENT 2
 * #define ESRCH 3
 * ...
 *
 * Additionally, man 3 errno says that on any linux machine there should be a
 * utility that offers the ability to BOTH list error numbers, and search for
 * them by keyword. On my xubuntu minimal install I had to install the
 * "moreutils" package. These just help the programmer decode faster
 *
 * $ errno -l
 * EPERM 1 Operation not permitted
 * ENOENT 2 No such file or directory
 * ESRCH 3 No such process
 * EINTR 4 Interrupted system call
 * ...
 *
 * $ errno 2
 * ENOENT 2 No such file or directory
 *
 * $ errno -s Network
 * ENONET 64 Machine is not on the network
 * ENOTUNIQ 76 Name not unique on network
 * ENETDOWN 100 Network is down
 * ENETUNREACH 101 Network is unreachable
 * ENETRESET 102 Network dropped connection on reset
 */ 
