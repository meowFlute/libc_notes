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
#include "stdlib.h" /* malloc */
#include "errno.h"  /* errno for malloc checking */
#include "error.h"  /* error for errno reporting */
#include "locale.h" /* LC_ALL, etc */

/* we'll use this dummy buffer length a few times */
#ifndef MAX_BUFF_LEN
#define MAX_BUFF_LEN 4
#endif

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

/* Section 5.3 Notes 
 * The main thing to remember here is to not accidentally set it up to where
 * you try to read the length of something with no null terminator */
void string_length_demo(void)
{
    printf( "\t===================\n"
            "\t=== Section 5.3 ===\n"
            "\t===================\n\n");

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
    printf("\n"); // end of section
    return;
}

/* something we'll use for more complex copying */
typedef struct _data {
    int int_data;
    char char_buff[MAX_BUFF_LEN];
    double double_data;
} data;

typedef struct _two_datas {
    data first_data;
    data second_data;
} two_datas;

/* Section 5.4 Notes
 * Number 1 takeaway: do not, under any circumstances, allow the source and
 * destination arrays to overlap */
void string_copying_demo(void)
{
    printf( "\t===================\n"
            "\t=== Section 5.4 ===\n"
            "\t===================\n\n");

    /* mem and str functions are in string.h */
    /* mem functions can be applied to much more than just strings */
    data from_data = { 12345, "678", 9.0 };
    data to_data = { 0 };

    printf( "to_data fields prior to memcpy:\n"
            "\t.int_data = %d\n"
            "\t.char_buff = %s\n"
            "\t.double_data = %lf\n",
            to_data.int_data,
            to_data.char_buff,
            to_data.double_data);
    
    /* technically returns a void * to the location of to_data */
    memcpy(&to_data, &from_data, sizeof(data));
    printf( "to_data fields prior to memcpy:\n"
            "\t.int_data = %d\n"
            "\t.char_buff = %s\n"
            "\t.double_data = %lf\n",
            to_data.int_data,
            to_data.char_buff,
            to_data.double_data);

    /* mempcpy returns a void * to the location just AFTER to, so something
     * like this where you write consecutively is suddenly possible */
    /* change to_data to something different than from_data */
    memcpy(to_data.char_buff, "123", MAX_BUFF_LEN);
    to_data.int_data = 321;
    to_data.double_data = 0.10987654321;
    /* allocate two struct _data's worth of memory */
    two_datas * both_data = malloc(2 * sizeof(data));
    if(both_data == NULL)
        error(EXIT_FAILURE, errno, "both_data allocation failed");
    /* copy them both into that new region with to_data in front */
    memcpy( mempcpy(both_data, &to_data, sizeof(data)), &from_data, sizeof(data));
    /* read both data structs */ 
    printf( "Reading two data structs written sequentially using mempcpy:\n"
            "\tboth_datas->first_data.int_data = %d\n"
            "\tboth_datas->first_data.char_buff = %s\n"
            "\tboth_datas->first_data.double_data = %lf\n"
            "\tboth_datas->second_data.int_data = %d\n"
            "\tboth_datas->second_data.char_buff = %s\n"
            "\tboth_datas->second_data.double_data = %lf\n",
            both_data->first_data.int_data,
            both_data->first_data.char_buff,
            both_data->first_data.double_data,
            both_data->second_data.int_data,
            both_data->second_data.char_buff,
            both_data->second_data.double_data);

    /* should you need to overlap for some reason, there is a function for that
     * called memmove */
    char buff[80] = "123456789 123456789 123456789";
    printf( "demo string we'll do an overlapped copy on:\n"
            "\t\"%s\"\n", buff);
    /* force it into an overlap scenario that still won't overflow the buffer
     * */
    int new_start = strlen(buff) - 15;
    printf( "copy target location: '%c' pos = %d\n",
            buff[new_start],
            new_start);
    /* perform the memmove */
    char * new_string = memmove(buff + new_start, buff, strlen(buff));
    printf( "new string value after blowing away the original null terminator "
            " with an overlapping copy:\n"
            "\t\"%s\"\n"
            "\tstrlen = %zu\n"
            "\tnew_string = \"%s\"\n",
            buff,
            strlen(buff),
            new_string);
    
    /* you can set a block of memory to an unsigned char value with memset */
    memset(both_data, 0, 2 * sizeof(data));
    printf( "Reading two data structs written to all zeros using memset:\n"
            "\tboth_datas->first_data.int_data = %d\n"
            "\tboth_datas->first_data.char_buff = %s\n"
            "\tboth_datas->first_data.double_data = %lf\n"
            "\tboth_datas->second_data.int_data = %d\n"
            "\tboth_datas->second_data.char_buff = %s\n"
            "\tboth_datas->second_data.double_data = %lf\n",
            both_data->first_data.int_data,
            both_data->first_data.char_buff,
            both_data->first_data.double_data,
            both_data->second_data.int_data,
            both_data->second_data.char_buff,
            both_data->second_data.double_data);

    /* strcpy we know, uses null terminator so I won't demo it */
    
    /* strdup uses malloc to allocate a new string in */
    /* strdupa is the same but uses alloca and requires GNU CC */
    char * heap_string = strdup("This goes on the heap!");
    if(heap_string == NULL)
        error(EXIT_FAILURE, errno, "heap_string allocation failed");
    printf( "String dynamically allocated using strdup:\n"
            "\t\"%s\"\n",
            heap_string);

    /* stpcpy is like strcpy but gives us the value of the null terminator */
    char concat_buffer[20] = { 0 };
    char *concat_ptr = concat_buffer;
    concat_ptr = stpcpy(concat_ptr, "12345");
    concat_ptr = stpcpy(concat_ptr, "67890");
    printf( "concatenated string using stpcpy:\n"
            "\t\"%s\"\n",
            concat_buffer);

    /* bcopy is more or less memmove but obsolete, and bzero is basically
     * memset but obsolete */

    /* don't forget to free your mallocs :) */
    free(heap_string);
    free(both_data);
    printf("\n");
}

/* Section 5.5 Notes 
 *
 * This section is very opinionated about the use of strcat! For example:
 *      "Programmers using the ‘strcat’ or ‘wcscat’ function (or the ‘strncat’
 *      or ‘wcsncat’ functions defined in a later section, for that matter) can
 *      easily be recognized as lazy and reckless."
 * 
 * Strong words!!!
 * 
 * Further it says:
 *      "Whenever a programmer feels the need to use ‘strcat’ she or he should
 *      think twice and look through the program to see whether the code cannot
 *      be rewritten to take advantage of already calculated results.  Again: it
 *      is almost always unnecessary to use ‘strcat’" 
 *
 * so we'll do a simple demo of both ways, memcpy (or mempcpy) and strcat here.
 * */
void string_concat_demo(void)
{
    printf( "\t===================\n"
            "\t=== Section 5.5 ===\n"
            "\t===================\n\n");

    /* two arbitrary strings */
    char * first_string = "Hello";
    char * second_string = ", world!";

    /* to put them somewhere we need to guarantee we have enough memory */
    size_t first_len = strlen(first_string);
    size_t second_len = strlen(second_string);
    size_t block_length = first_len + second_len + 1;
    char * dest_memcpy = malloc(block_length);
    if(dest_memcpy == NULL)
        error(EXIT_FAILURE, errno, "dest_memcpy allocation error");
    char * dest_strcat = malloc(block_length);
    if(dest_strcat == NULL)
        error(EXIT_FAILURE, errno, "dest_strcat allocation error");

    /* strcat method of adding them in */
    strcpy(dest_strcat, first_string);
    strcat(dest_strcat, second_string); // this function has to find the end of
                                        // the first string manually so it can 
                                        // concat

    /* memcpy method of adding them in */
    /* note how because we already found the lengths to make the buffer, we
     * didn't need to make strcat find the lengths again */
    memcpy( mempcpy(dest_memcpy, first_string, first_len), 
            second_string, 
            second_len);
    dest_strcat[first_len+second_len] = '\0';

    /* output to verify string contents and concat success */
    printf( "strcat vs. memcpy strings:\n"
            "\tstrcat: \"%s\"\n"
            "\tmemcpy: \"%s\"\n",
            dest_strcat,
            dest_memcpy);

    /* free dynamically allocated memory */
    free(dest_strcat);
    free(dest_memcpy);

    /* so ya, if you go through all the trouble of dynamically allocating
     * memory in the right way to verify that the buffer can always hold the
     * string then you already did all the work to just do a memcpy. I guess
     * that probably also goes for most of the strcpy functions as well */
    printf("\n");
}

/* Section 5.6 Notes 
 * Truncation is just as spicy as concatenation, and for good reason:
 *      "Because these functions can abruptly truncate strings or wide
 *      strings, they are generally poor choices for processing text.  When
 *      coping or concatening multibyte strings, they can truncate within a
 *      multibyte character so that the result is not a valid multibyte string.
 *      When combining or concatenating multibyte or wide strings, they may
 *      truncate the output after a combining character, resulting in a
 *      corrupted grapheme.  They can cause bugs even when processing
 *      single-byte strings: for example, when calculating an ASCII-only user
 *      name, a truncated name can identify the wrong user."
 *
 * The manual suggests using instead -fcheck-pointer-bounds and
 * -fsanitize=address options with gcc to debug instead of truncating strings.
 *
 * This makes sense, because you'll just roll yourself into new bugs since
 * strncpy doesn't do null termination and things like strndup which do add a
 * null terminator can stull truncate a unicode character into something
 * nonsensical.
 *
 * GNU Coding Standards suggest just using dynamic memory allocation carefully
 * and never truncating for buffer length.
 * */
void string_truncate_demo(void)
{
    /* not demo'd because I don't plan on using them */
    /* These include functions like strncpy or strndup/strndupa */
}

/* Section 5.7 Notes */
void string_compare_demo(void)
{
    printf( "\t===================\n"
            "\t=== Section 5.7 ===\n"
            "\t===================\n\n");

    /* memcmp is really only useful for something where you want to compare
     * exact blocks, as it takes each byte as an unsigned char and promotes it
     * to int to do the comparison. You might be tempted to use it to compare
     * something like a struct or buffer but you have to be careful of padded
     * memory that isn't necessarily initialized to zero throwing things off */
    char bz[10] = { 0 };
    char bn[10];
    char * control = "test";
    strcpy(bz, control);
    strcpy(bn, control);
    
    printf( "comparing 'identical' buffers with memcmp: %d\n", 
            memcmp(bz, bn, 10));
    printf( "inspecting each byte of 'identical' buffers containing \"%s\":\n"
            "\t{%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x}\n"
            "\t{%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x}\n",
            control,
            bz[0],bz[1],bz[2],bz[3],bz[4],bz[5],bz[6],bz[7],bz[8],bz[9],
            bn[0],bn[1],bn[2],bn[3],bn[4],
            (unsigned char)bn[5],(unsigned char)bn[6],(unsigned char)bn[7],
            (unsigned char)bn[8],(unsigned char)bn[9]);

    /* now we demo strcmp */
    printf( "comparing the same buffers with strcmp: %d\n", strcmp(bz, bn));

    /* strncmp exists to control considered length */
    printf( "comparing the first %d characters using strncmp: %d\n", 
            5, strncmp(bz, bn, 5));

    /* strcasecmp and strncasecmp can ignore case of multibyte characters in a
     * locale dependent way */
    strcpy(bn, "TEST");
    printf( "comparing %s and %s using strcasecmp: %d\n", 
            bz, bn, strcasecmp(bz, bn));

    /* a gnu extension to compare versions of filenames or whatever exists
     * strverscmp -- I don't see myself ever using this */

    printf("\n");
}

/* Section 5.8 Notes 
 * collating is useful when you end up using qsort or something similar because
 * if provides a locale dependent method comparing strings and fitting their
 * characters into the correct alphabetical order */
int compare_elements (const void * s1, const void * s2) 
{
    /* read these right to left */
    /* a pointer to a (const void) being cast as a pointer to a const pointer
     * to a char, which when we dereference in the strcoll function is just a
     * const pointer to a char. */
    /* strcoll expects char *, and const void * can't be cast without
     * discarding things unless we do the second const */
    char * const * p1 = s1;
    char * const * p2 = s2;
    return strcoll(*p1, *p2); 
}
void string_collate_demo(void)
{
    printf( "\t===================\n"
            "\t=== Section 5.8 ===\n"
            "\t===================\n\n");

    /* strcoll compares two strings, strxfrm transforms a string */
    char * str_array[4] = { "Hello", "hello", "friday", "blurb" };
    printf( "Array order prior to collated sort:\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n",
            str_array[0],str_array[1],str_array[2],str_array[3]);

    /* run them with the en_US.UTF-8 locale */
    printf("setting locale to en_US.UTF-8\n");
    setlocale(LC_ALL, "en_US.UTF-8");
    printf("strcoll(\"h\",\"H\") = %d\n",strcoll("h","H"));
    qsort(str_array, 4, sizeof(char *), compare_elements);
    printf( "Array order after collated sort:\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n",
            str_array[0],str_array[1],str_array[2],str_array[3]);

    /* running again with C.UTF-8 */
    printf("setting locale to C.UTF-8\n");
    setlocale(LC_ALL, "C.UTF-8");
    printf("strcoll(\"h\",\"H\") = %d\n",strcoll("h","H"));
    qsort(str_array, 4, sizeof(char *), compare_elements);
    printf( "Array order after collated sort:\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n"
            "\t\"%s\"\n",
            str_array[0],str_array[1],str_array[2],str_array[3]);
    
    /* strxfrm demo */

    /* I appear to be experiencing a UTF-8 bug like what is described here 
     * https://stackoverflow.com/questions/51943128/how-to-use-strxfrm-in-c-language
     */
    /* if you pass size of 0 it'll return what size it would have needed for a
     * transformed string */
    // setlocale(LC_ALL, "en_US.UTF-8");
    // int len = strxfrm(NULL, "hello", 0);
    // char * transformed_hello = malloc(len);
    // if(transformed_hello == NULL)
    //     error(EXIT_FAILURE, errno, "xfrm_hello allocation failed");
    // strxfrm(transformed_hello, "hello", len-1);
    // printf( "%s transformed into %s with strxfrm\n",
    //         "hello",
    //         transformed_hello);
    printf("\n");
}

/* Section 5.9 Notes 
 * No specific drama in the notes here, and these seem like very useful
 * functions. One thing to be aware of is that they often consider bytes so be
 * careful with multibyte strings */
void string_search_demo(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    printf( "\t===================\n"
            "\t=== Section 5.9 ===\n"
            "\t===================\n\n");
    
    /* UTF-8 U+21D2 RIGHTWARDS DOUBLE ARROW as octal escape strings */
    char * arrow = "\342\207\222";

    /* *chr functions find a char (or unsigned char for mem functions) 
     * *rchr starts from the end of the block 
     * strchrnul returns the end of the string instead of NULL if it doesn't
     * find anything */
    /* for instance if I wanted a pointer to the null terminator */
    char * end_of_arrow = strchr(arrow, '\0');
    printf( "string \"%s\" at %p has a null termination character at %p\n",
            arrow,
            arrow,
            end_of_arrow);

    /* we'll search for a specific set of memory using memmem */
    union float_or_byte_array
    {
        float f;
        unsigned char uchr[4];
    };
    union float_or_byte_array foba;
    foba.f = 2.35f;
    struct struct_of_floats
    {
        float f1;
        float f2;
        float f3;
    };
    struct struct_of_floats haystack = { 1.23f, 2.35f, 3.405f };

    float * needle = memmem(&haystack, 
                            sizeof(struct struct_of_floats), 
                            foba.uchr,
                            4);

    printf( "ptr to float of value %f found in %p of len %zu using memmem:\n"
            "\t%s %p = %f\n",
            foba.f,
            &haystack,
            sizeof(struct struct_of_floats),
            arrow,
            needle,
            *needle);

    /* strstr does the same thing, but with substrings 
     * strcasestr does a case insensitive search */
    char * long_string = "The quick brown fox jumps over the lazy dog";
    char * substring = "lazy";
    char * result = strstr(long_string, substring);
    printf("Substring %s found at %p: %s\n", substring, result, result);

    /* strspn/strcspn/strpbrk is almost like a regex search, but you specifiy every
     * character in the class explicitly.
     *
     * the first two return a length of characters matched and *pbrk returns a
     * pointer to the break point */
    size_t num_matches = strspn(result, "abcdefghijklmnopqrstuvwxyz");
    printf("\"%s\" has %zu lowercase letters sequentially at the start\n",
            result,
            num_matches);

    printf("\n");
}

/* Section 5.10 Notes */
void string_split_demo(void)
{
    printf( "\t====================\n"
            "\t=== Section 5.10 ===\n"
            "\t====================\n\n");

    /* strtok is common to use, but keep in mind that it is MT-Unsafe due to a
     * race condition based on it NOT being reentrant (it uses static, global,
     * or dynamically allocated memory it finds for itself and as a result two
     * calls to the same function can interfere with one another.
     *
     * It is also important to note that strtok MODIFIES the string you pass to
     * it as an argument */
    char * string_with_tokens = "you, me, someone else, yo mama";
    char * tmp_copy = strdupa(string_with_tokens);
    if(tmp_copy == NULL)
        error(EXIT_FAILURE, errno, "tmp_copy allocation failed");
    char * token = strtok(tmp_copy, ",");
    printf( "Finding tokens in \"%s\" separated by \",\":\n", 
            string_with_tokens);
    while(token != NULL)
    {
        /* we'll leave token alone */
        char * tokptr = token;
        /* trim leading spaces */
        while(tokptr[0] == ' ')
        {
            tokptr++;
        }
        printf("\t\"%s\"\n", tokptr);
        token = strtok(NULL, ",");
    }

    /* reentrance is overcome in the function strtok_r, which has a third
     * argument that stores the result. strtok_r is part of POSIX.1 and is
     * multi-threading safe */
    tmp_copy = strdupa(string_with_tokens);
    if(tmp_copy == NULL)
        error(EXIT_FAILURE, errno, "tmp_copy second allocation failed");
    char * save_ptr;
    token = strtok_r(tmp_copy, ",", &save_ptr);
    printf( "Finding tokens again, this time with reentrant strtok_r:\n");
    while(token != NULL)
    {
        /* we'll leave token alone */
        char * tokptr = token;
        /* trim leading white space */
        while(tokptr[0] == ' ')
        {
            tokptr++;
        }
        printf("\t\"%s\"\n", tokptr);
        token = strtok_r(NULL, ",", &save_ptr);
    }

    /* perhaps even simpler is strsep, which takes the third arg and the first
     * arg of strtok_r and combined them */
    tmp_copy = strdupa(string_with_tokens);
    if(tmp_copy == NULL)
        error(EXIT_FAILURE, errno, "tmp_copy second allocation failed");
    token = strsep(&tmp_copy, ",");
    printf("Once more, but this time with strsep:\n");
    while(token != NULL)
    {
        char * tokptr = token;    
        while(tokptr[0] == ' ')
        {
            tokptr++;
        }
        printf("\t\"%s\"\n", tokptr);
        token = strsep(&tmp_copy, ",");
    }

    /* basename and dirname exist in this space too, I won't worry about it for
     * now, but I'll keep that in mind */

    printf("\n");
}

/* Section 5.11 Notes 
 * This section is short because there is only once concept and one function
 * explicit_bzero is meant to be an erase call **THAT WON'T BE OPTIMIZED AWAY
 * BY THE COMPILER** 
 *
 * So simple as that, if you have sensitive data like a private key or
 * something and you want to try to minimize time that it is in RAM you can use
 * explicit_bzero, because memset or bzero alone will likely be optimized away
 * */
void get_password(char * buffer, size_t max_length)
{
    strncpy(buffer, "test", max_length);
}
void string_erasing_demo(void)
{
    char pass[40];
    get_password(pass, 40);
    /* do something with the password */
    /* look at this with a debugger so that you can see it happen */
    explicit_bzero(pass, 40);
}

/* Section 5.12 Notes 
 * This demo just jumbles up the string irreversably. If you want to erase it
 * use explicit_bzero, if you want to obfuscate it reversibly use memfrob */
void string_shuffle_demo(void)
{
    printf( "\t====================\n"
            "\t=== Section 5.12 ===\n"
            "\t====================\n\n");

    char * unshuffled_string = "The quick brown fox jumped over the lazy dog.";
    char * shuffled_string = strfry(strdupa(unshuffled_string));
    printf( "\"%s\" strfry'd to become:\n"
            "\t\"%s\"\n", 
            unshuffled_string,
            shuffled_string);

    printf("\n");
}

/* Section 5.13 Notes 
 * not super useful, but totally reversable */
void string_obfuscate_demo(void)
{
    printf( "\t====================\n"
            "\t=== Section 5.12 ===\n"
            "\t====================\n\n");

    char * unobf_string = "The quick brown fox jumped over the lazy dog.";
    size_t unobf_len = strlen(unobf_string);
    char * obf_string = strdupa(unobf_string);
    memfrob(obf_string, unobf_len);

    printf( "original string: \"%s\"\n"
            "\tmemfrob'd string: \"%s\"\n",
            unobf_string,
            obf_string);

    printf( "\tmemfrob^2'd string: \"%s\"\n",
            (char *)memfrob(obf_string, unobf_len));

    printf("\n");
}

/* Section 5.14 Notes */
void string_encode_demo(void)
{

}

/* Section 5.15 Notes */
void string_argz_envz_demo(void)
{

}
