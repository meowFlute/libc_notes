/* General Notes 5.1 & 5.2 
 * 5.1 - Representations of Strings
 *
 * One of the major common pitfalls of using strings is messing up the way
 * multibyte characters fit into fixed-width buffers. When many of the string
 * utilities in this section were written, ASCII characters were the characters
 * considered, not UTF-8. This means that they consider all characters to be
 * 1-byte length characters, when in reality UTF-8 characters can be up to 4
 * bytes in length.
 *
 * Obviously if you are not careful when uting UTF-8 character encoding with
 * legacy string utilities it is very much a possibility that you could exceed
 * buffer lengths on read or write operations 
 *
 * The problem is wchar_t isn't really fixed-width, because on AIT and windows
 * machines wchar_t is 16-bit and can't accomodate all of the unicode
 * characters. So they're either limited to UCS-2, or they suddenly have a
 * multi-byte problem like before.
 *
 * To solve this we use libunistring which has solved these problems
 *
 * 5.2 - Strings and Array conventions 
 *
 * in libc, functions that operate on arbitrary blocks of memory start with mem
 * or wmem (such as memcpy) and these invariably take arguments specifying size
 * in bytes (or wide characters, which are fixed-width regardless of encoding
 * standard. They can operate on any kind of pointer and get the correct size
 * to them using sizeof, but wmem are specially reserved for (wchar_t *)
 * pointer types and are not really usable with anything but arrays of this
 * type 
 *
 * If the function starts with str or wcs (e.g. strcpy or wcscpy) then they
 * look for null-termination to figure out size. 
 *
 * IF THE SIZE IS KNOWN, USE MEM FUNCTIONS TO REDUCE EDGE CASE BUGS 
 *
 * Alternatively you could look into a UTF standard library like libunistring 
 *      I installed this with 'sudo apt install libunistring-dev' 
 * */

#include "05_string_utils.h"    

#include "stdio.h"  /* printf */
#include "string.h" /* most other functions used here for char strings */
#include "wchar.h"  /* wcslen, etc */

/* run all of the subsections in order */
void string_run_demos(void)
{
    string_length_demo();
    string_copying_demo();
    string_concat_demo();
    string_truncate_demo();
    string_compare_demo();
    string_collate_demo();
    string_search_demo();
    string_split_demo();
    string_erasing_demo();
    string_shuffle_demo();
    string_obfuscate_demo();
    string_encode_demo();
    string_argz_envz_demo();
}

/* Section 5.3 Notes */
void string_length_demo(void)
{
    char demo_buffer[80] = { 0 };
    char * demo_string = "hello, world!";
   
    /* copy "hello, world!" into a much larger buffer */
    strcpy(demo_buffer, demo_string);

    /* demo how strlen != sizeof in such a case */
    printf("strlen is based on the null termination character, not the size\n");
    printf( "for example:\n\tsizeof(demo_buffer[80]) = %zu\n\t"
            "strlen(demo_buffer[80]) = %zu\n",
            sizeof(demo_buffer),
            strlen(demo_buffer));

    printf( "For a char * demo_string pointing to the same contents:\n\t"
            "sizeof(demo_string) = %zu\n\t"
            "strlen(demo_string) = %zu\n",
            sizeof(demo_string),
            strlen(demo_string));
   
    /* there are wide character equivalents as well */
    wchar_t demo_wide_buffer[80] = { 0 };
    wchar_t * demo_wide_string = L"hello, world!";

    wcscpy(demo_wide_buffer, demo_wide_string);

    printf( "Now using a wide char string:\n\t"
            "sizeof(demo_wide_buffer) = %zu\n\t"
            "wcslen(demo_wide_buffer) = %zu\n",
            sizeof(demo_wide_buffer),
            wcslen(demo_wide_buffer));

    printf( "Now using a wide char string:\n\t"
            "sizeof(demo_wide_string) = %zu\n\t"
            "wcslen(demo_wide_string) = %zu\n",
            sizeof(demo_wide_string),
            wcslen(demo_wide_string));

    /* to try to mitigate the nastiness of multibyte strings you can protect
     * using strnlen which specified a maximum length to avoid overflow */
#define MAX_BUFF_LEN 4
    /* oh no, we forgot the null terminator '\0' !!! */
    char overflowed_buffer[MAX_BUFF_LEN] = { 'a', 'b', 'c', 'd' };
    printf( "This is how you protect from an overrun when the buffer doesn't "
            "have room for a null terminator character:\n\t"
            "sizeof(overflowed_buffer[4]) = %zu\n\t"
            "strnlen(overflowed_buffer, 4) = %zu\n\t"
            "strlen(overflowed_buffer) = %zu <--- if this is larger than 4 we "
            "are reading outside of the array boundaries in strlen when looking"
            " for the null termination character!!!\n",
            sizeof(overflowed_buffer),
            strnlen(overflowed_buffer, MAX_BUFF_LEN),
            strlen(overflowed_buffer));

    /* the same exists as a gnu extension for wide chars as wcsnlen */
}

/* Section 5.4 Notes */
void string_copying_demo(void)
{

}

/* Section 5.5 Notes */
void string_concat_demo(void)
{

}

/* Section 5.6 Notes */
void string_truncate_demo(void)
{

}

/* Section 5.7 Notes */
void string_compare_demo(void)
{

}

/* Section 5.8 Notes */
void string_collate_demo(void)
{

}

/* Section 5.9 Notes */
void string_search_demo(void)
{

}

/* Section 5.10 Notes */
void string_split_demo(void)
{

}

/* Section 5.11 Notes */
void string_erasing_demo(void)
{

}

/* Section 5.12 Notes */
void string_shuffle_demo(void)
{

}

/* Section 5.13 Notes */
void string_obfuscate_demo(void)
{

}

/* Section 5.14 Notes */
void string_encode_demo(void)
{

}

/* Section 5.15 Notes */
void string_argz_envz_demo(void)
{

}
