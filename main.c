#include "includes/libft_malloc_ubuntu.h"

int main(int ac, char **av, char **envp)
{
    void *a;
    a = malloc(10);
    a = realloc(a, 19);
    free(a);
    return 0;
}