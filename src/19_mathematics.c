#include "19_mathematics.h"

#include "math.h"
#include "stdio.h"

static void constants(void);
static void trig(void);
static void inverse_trig(void);
static void exponents_logs(void);
static void hyperbolics(void);
static void specials(void);
static void errors_in_math(void);
static void pseudo_random(void);
static void function_optimizations(void);

void mathematics_run_demos(void)
{
    constants();
    trig();
    inverse_trig();
    exponents_logs();
    hyperbolics();
    specials();
    errors_in_math();
    pseudo_random();
    function_optimizations();
}

static void constants(void)
{
    printf( "==================\n"
            "== SECTION 19.1 ==\n"
            "=== CONSTANTS ====\n"
            "==================\n\n");

    printf("Double Constants at 14 places of precision:\n");
    printf("\tM_E        = %.14lf\n", M_E);
    printf("\tM_LOG2E    = %.14lf\n", M_LOG2E);
    printf("\tM_LOG10E   = %.14lf\n", M_LOG10E);
    printf("\tM_LN2      = %.14lf\n", M_LN2);
    printf("\tM_LN10     = %.14lf\n", M_LN10);
    printf("\tM_PI       = %.14lf\n", M_PI);
    printf("\tM_PI_2     = %.14lf\n", M_PI_2);
    printf("\tM_PI_4     = %.14lf\n", M_PI_4);
    printf("\tM_1_PI     = %.14lf\n", M_1_PI);
    printf("\tM_2_PI     = %.14lf\n", M_2_PI);
    printf("\tM_2_SQRTPI = %.14lf\n", M_2_SQRTPI);
    printf("\tM_SQRT2    = %.14lf\n", M_SQRT2);
    printf("\tM_SQRT1_2  = %.14lf\n", M_SQRT1_2);

    printf("Long double pi example at 34 places of precision:\n");
    printf("\tM_PIl      = %0.34Lf\n", M_PIl);

    printf("float pi example at 6 places of precision:\n");
    printf("\tM_PIf      = %0.6f\n", M_PIf);
    printf("\n");
}

static void trig(void)
{

}

static void inverse_trig(void)
{

}

static void exponents_logs(void)
{

}

static void hyperbolics(void)
{

}

static void specials(void)
{

}

static void errors_in_math(void)
{

}

static void pseudo_random(void)
{

}

static void function_optimizations(void)
{

}
