#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

extern t_heap g_heap;


int check_available_alloc(void *ptr, size_t size)
{
    t_page *temp;
    if (size >= aw)
}

void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return(sort_allocations(size));
    check_available_alloc(ptr, size);
}