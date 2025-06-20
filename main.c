#include "includes/libft_malloc_ubuntu.h"

int main()
{
    //printf("why god 1\n");
    void *a = malloc(1000);
    //printf("why god 2\n");
    void *b = malloc(100);
    //printf("why god 3\n");
    void *c = malloc(1);
    //printf("why god 4\n");
    
    show_alloc_mem();
    //printf("here\n");

    free(c);
    show_alloc_mem();
    return (0);
}