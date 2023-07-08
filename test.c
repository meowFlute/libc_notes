#include <stdint.h>
#include <stdio.h>

int main()
{
    size_t size_t_max = SIZE_MAX;
    size_t size_t_min = 0;

    printf("SIZE_MAX is %zu\n", size_t_max);
    printf("here is what happens if you subtract 1 from zero with size_t: \
            %zu\n", size_t_min - 1);

}
