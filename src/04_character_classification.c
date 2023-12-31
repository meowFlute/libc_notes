/* Chapter 4 -- Character Handling
 *
 * ***ctype.h*** doesn't refer to types used in the C language, but instead to
 * "character" types. It provides utilities on how to bin them so various
 * standard classes (digit, alpha, etc).
 *
 * The choice of locale and character set can alter the classification of
 * character codes, so all of these functions are impacted by choice of locale,
 * more specifically the LC_CTYPE category.
 *
 * ISO C specifies another set of functions for wide characters, otherwise
 * known as wchar_t. Perhaps unsurprisingly, the header for these types are
 * found in ***wctype.h***
 * */

#include "04_character_classification.h"
#include <stdio.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

/* Section 4.1 -- Classification of Characters
 * All of the functions in this demo are from ctype.h, and they take an int
 * character as an argument and return an int as a success bool so you can use
 * them in if statements directly 
 *
 * An additional note from the manpage isalpha(3):
 *      The standards require that the argument c for these functions is either
 *      EOF or a value that is representable in the type unsigned char.  If the
 *      argument c is of type char, it must be cast to unsigned char, as in the
 *      following example:
 *
 *          char c;
 *          ...
 *          res = toupper((unsigned char) c);
 *
 *      This is necessary because char may be the equivalent of signed char, in
 *      which case a byte where the top bit is set would be sign extended  when
 *      converting  to  int,  yielding a value that is outside the range of un‐
 *      signed char.
 *
 * We find the following classification functions in ctype.h:
 * - int islower( int C )
 *      Returns true if C is a lower-case letter.  The letter need not be
 *      from the Latin alphabet, any alphabet representable is valid.
 *
 * - int isupper( int C )
 *      Returns true if C is an upper-case letter.  The letter need not be
 *      from the Latin alphabet, any alphabet representable is valid.
 * 
 * - int isalpha( int C )
 *      Returns true if C is an alphabetic character (a letter).  If
 *      ‘islower’ or ‘isupper’ is true of a character, then ‘isalpha’ is
 *      also true.
 *
 *      In some locales, there may be additional characters for which
 *      ‘isalpha’ is true—letters which are neither upper case nor lower
 *      case.  But in the standard ‘"C"’ locale, there are no such
 *      additional characters.
 * 
 * - int isdigit( int C )
 *      Returns true if C is a decimal digit '0' through '9'
 * 
 * - int isalnum( int C ) 
 *      isalpha OR'd with isdigit
 *
 * - int isxdigit( int C )
 *      Returns true if C is a hexadecimal digit.  Hexadecimal digits
 *      include the normal decimal digits ‘0’ through ‘9’ and the letters
 *      ‘A’ through ‘F’ and ‘a’ through ‘f’.
 *
 * - int ispunct( int C )
 *      Returns true if C is a punctuation character.  This means any
 *      printing character that is not alphanumeric or a space character.
 *
 * - int isspace( int C )
 *      Returns true if C is a “whitespace” character.  In the standard
 *      ‘"C"’ locale, ‘isspace’ returns true for only the standard
 *      whitespace characters:
 * 
 *      ‘' '’
 *           space
 * 
 *      ‘'\f'’
 *           formfeed
 * 
 *      ‘'\n'’
 *           newline
 * 
 *      ‘'\r'’
 *           carriage return
 * 
 *      ‘'\t'’
 *           horizontal tab
 * 
 *      ‘'\v'’
 *           vertical tab
 * 
 * - int isblank( int C )
 *      Returns true if C is a blank character; that is, a space or a tab.
 *      This function was originally a GNU extension, but was added in
 *      ISO C99.
 *
 * - int isgraph( int C )
 *      Returns true if C is a graphic character; that is, a character that
 *      has a glyph associated with it.  The whitespace characters are not
 *      considered graphic.
 *
 * - int isprint( int C )
 *      Returns true if C is a printing character.  Printing characters
 *      include all the graphic characters, plus the space (‘ ’) character.
 *
 * - int iscntrl( int C )
 *      Returns true if C is a control character (that is, a character that
 *      is not a printing character).
 *
 * - int isascii( int C )
 *      Returns true if C is a 7-bit ‘unsigned char’ value that fits into
 *      the US/UK ASCII character set.  This function is a BSD extension
 *      and is also an SVID extension.
 */
void char_classification_demo(void)
{
    /* get used to casting this to an unsigned char EVERY FUNCTION CALL lol,
     * otherwise it is undefined behavior */
    char * test_string = "UPPER lower alph4num3r1c 0xDEADbeef1337\n\t";
    int i;

    /* This for loop runs until the null terminator */
    for(i = 0; test_string[i]; i++)
    {
        printf("char test_string[i] = '%c'\n", test_string[i]);
        printf("\tint value = %d\n", (int)test_string[i]);
        if(((int)test_string[i]) != ((int)((unsigned char)test_string[i])))
            printf("\t====>>int value after unsigned char cast: %d", (int)((unsigned char)test_string[i]));
        printf("\tislower = %d\n", islower((unsigned char)test_string[i]));
        printf("\tisupper = %d\n", isupper((unsigned char)test_string[i]));
        printf("\tisalpha = %d\n", isalpha((unsigned char)test_string[i]));
        printf("\tisdigit = %d\n", isdigit((unsigned char)test_string[i]));
        printf("\tisalnum = %d\n", isalnum((unsigned char)test_string[i]));
        printf("\tisxdigit = %d\n", isxdigit((unsigned char)test_string[i]));
        printf("\tispunct = %d\n", ispunct((unsigned char)test_string[i]));
        printf("\tisspace = %d\n", isspace((unsigned char)test_string[i]));
        printf("\tisblank = %d\n", isblank((unsigned char)test_string[i]));
        printf("\tisgraph = %d\n", isgraph((unsigned char)test_string[i]));
        printf("\tisprint = %d\n", isprint((unsigned char)test_string[i]));
        printf("\tiscntrl = %d\n", iscntrl((unsigned char)test_string[i]));
        printf("\tisascii = %d\n", isascii((unsigned char)test_string[i]));
    }

    /* let's see what this breaks */
    signed char whoopsies;
    int islowercntr = 0;
    int isuppercntr = 0;
    int isalphacntr = 0;
    int isdigitcntr = 0;
    int isalnumcntr = 0;
    int isxdigitcntr = 0;
    int ispunctcntr = 0;
    int isspacecntr = 0;
    int isblankcntr = 0;
    int isgraphcntr = 0;
    int isprintcntr = 0;
    int iscntrlcntr = 0;
    int isasciicntr = 0;

    /* try out all of th negative values and bail when it wraps */
    for(whoopsies = -1; whoopsies < 0; whoopsies--)
    {
        /* purposefully messed up values */ 
        if(isxdigit(whoopsies)) isxdigitcntr++;
        if(islower(whoopsies)) islowercntr++;
        if(isupper(whoopsies)) isuppercntr++; 
        if(isalpha(whoopsies)) isalphacntr++; 
        if(isdigit(whoopsies)) isdigitcntr++; 
        if(isalnum(whoopsies)) isalnumcntr++; 
        if(ispunct(whoopsies)) ispunctcntr++; 
        if(isspace(whoopsies)) isspacecntr++; 
        if(isblank(whoopsies)) isblankcntr++; 
        if(isgraph(whoopsies)) isgraphcntr++; 
        if(isprint(whoopsies)) isprintcntr++; 
        if(iscntrl(whoopsies)) iscntrlcntr++; 
        if(isascii(whoopsies)) isasciicntr++; 
    }        

    /* seems safe in my locale */
    printf("undefined behavior counters in my locale:\n");
    printf("\tislowercntr = %d\n", islowercntr); 
    printf("\tisuppercntr = %d\n", isuppercntr); 
    printf("\tisalphacntr = %d\n", isalphacntr); 
    printf("\tisdigitcntr = %d\n", isdigitcntr); 
    printf("\tisalnumcntr = %d\n", isalnumcntr); 
    printf("\tisxdigitcntr = %d\n", isxdigitcntr);
    printf("\tispunctcntr = %d\n", ispunctcntr); 
    printf("\tisspacecntr = %d\n", isspacecntr); 
    printf("\tisblankcntr = %d\n", isblankcntr); 
    printf("\tisgraphcntr = %d\n", isgraphcntr); 
    printf("\tisprintcntr = %d\n", isprintcntr); 
    printf("\tiscntrlcntr = %d\n", iscntrlcntr); 
    printf("\tisasciicntr = %d\n", isasciicntr); 

    return;
}

/* Ignoring the SVID compatibility function repeats, you have 3 options for
 * mappings
 * 1) int tolower(int C)
 * 2) int toupper(int C)
 * 3) int toascii(int C)
 * */
void char_case_conversion_demo(void)
{
    char * some_chars = "Here ÃrE sõmé chAracters";
    char upperbuf[80] = {0,};
    char lowerbuf[80] = {0,};
    char asciibuf[80] = {0,};

    /* go until the null terminator of the string */
    for(int i = 0; some_chars[i]; i++)
    {
        upperbuf[i] = toupper((unsigned char)some_chars[i]);
        lowerbuf[i] = tolower((unsigned char)some_chars[i]);
        asciibuf[i] = toascii((unsigned char)some_chars[i]);
    }

    /* set null byte manually before trying to read */
    upperbuf[strlen(some_chars)] = '\0';
    lowerbuf[strlen(some_chars)] = '\0';
    asciibuf[strlen(some_chars)] = '\0';

    /* note how the above don't really work on the funny characters */
    printf("original string:%s\nupper: %s\nlower: %s\nascii: %s\n",
            some_chars,
            upperbuf,
            lowerbuf,
            asciibuf);

    return;
}

/* different locales have different classifications defined for them, but the
 * definitions guaranteed to exist in every local are:
 * 
 *   ‘"alnum"’          ‘"alpha"’          ‘"cntrl"’          ‘"digit"’
 *   ‘"graph"’          ‘"lower"’          ‘"print"’          ‘"punct"’
 *   ‘"space"’          ‘"upper"’          ‘"xdigit"’
 * 
 * you can access these tests either by using commonly-used predefined
 * functions which have a form like 
 *      int iswalnum(wint_t WC) 
 * 
 * you can also access them by a new flow that is:
 *  -> wctype_t wctype (const char *PROPERTY)
 *  -> int iswctype(wint_t WC, wctype_t DESC)
 * which would look something like this when casting a single-byte char c:
 *      int is_in_class (int c, const char *class)
 *      {
 *          wctype_t desc = wctype (class);
 *          return desc ? iswctype (btowc (c), desc) : 0;
 *      }
 */
void wchar_classification_demo(void)
{
    /* I don't know why yet but I need to set the locale before I can use wide
     * chars */
    setlocale(LC_ALL, "C.UTF-8");

    wchar_t * wide_chars = L"Here ÃrE sõmé chAracters";
    wchar_t upper[30];
    wchar_t lower[30];
    size_t ucntr = 0;
    size_t lcntr = 0;

    /* size_t wcslen(const wchar_t * WS) is the equivalent of strlen */
    for(size_t i = 0; i < wcslen(wide_chars); i++)
    {
        /* show how to do it using iswctype */
        if(iswctype(wide_chars[i], wctype("upper")))
            upper[ucntr++] = wide_chars[i];

        /* show how to do it using the iswlower function */
        if(iswlower(wide_chars[i]))
            lower[lcntr++] = wide_chars[i];
    }
    upper[ucntr] = L'\0';
    lower[lcntr] = L'\0';

    printf("Original String: %ls\n\tUpper: %ls\n\tLower: %ls\n",
            wide_chars,
            upper,
            lower);

    return;
}

/* This demo is just that you can't simply cast a char to wchar_t, use btowc
 * instead (see also chapter 6) */
void wchar_usage_demo(void)
{
    /* as a contrived example I'll use a wide char comparison on a string made
     * of chars */
    char * some_chars = "aeiou234";
    wctype_t desc = wctype("digit");
    
    printf("digits in wide string %s : ", some_chars);
    for(unsigned long i = 0; i < strlen(some_chars); i++)
    {
        /* check the wide char type by converting the char to a wide char, also
         * test whether desc is 0 and return false (0) if it is */
        if(desc ? iswctype (btowc(some_chars[i]), desc) : false)
            printf("%c", some_chars[i]);
    }
    printf("\n");

    return;
}

/* here we show the two methods you can use to map with wide characters */
void wchar_mapping_demo(void)
{
    setlocale(LC_ALL, "C.UTF-8");
    wchar_t * wide_chars = L"Here ÃrE sõmé chAracters";
    printf("wchar mapping demo string: %ls\n", wide_chars);

    printf("Converting to upper: ");
    for(size_t i = 0; i < wcslen(wide_chars); i++)
    {
        /* demo of how to do it for any class conversion */
        printf("%lc", towctrans(wide_chars[i], wctrans("toupper")));
    }
    printf("\n");

    printf("Converting to lower: ");
    for(size_t i = 0; i < wcslen(wide_chars); i++)
    {
        /* demo of how to do it using built ins */
        printf("%lc", towlower(wide_chars[i]));
    }
    printf("\n");

    return;
}

