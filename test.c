#include <stdlib.h>
#include <malloc/malloc.h>
#include <stdio.h>

int main()
{
    void *p = malloc(0);
    size_t usable = malloc_size(p);
    printf("%p\n", usable);
}