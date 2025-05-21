#include "malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

extern t_heap g_heap;

void *realloc(void *ptr, size_t size)
{
}