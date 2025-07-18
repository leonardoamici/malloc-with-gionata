#include "includes/libft_malloc_ubuntu.h"

int main()
{
    char *ptr = malloc(10);
    for (int i = 0; i < 10; i++) ptr[i] = 'A' + i;
    ptr = realloc(ptr, 20);
    for (int i = 0; i < 10; i++) putchar(ptr[i]);
    ft_printf("here\n");
    print_hexa_dump(ptr);
    return 0;
}