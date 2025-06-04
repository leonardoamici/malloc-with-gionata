#include "../includes/malloc.h"

extern t_heap g_heap;

void ft_free(void *ptr)
{
    t_chunk *chunk;

    if (!ptr)
        return;

    chunk = g_heap.tiny.head;
    while (chunk)
    {
        if (chunk->head == ptr)
        {
            chunk->available = 1;
            chunk->freed = 1;
            return;
        }
        chunk = chunk->next;
    }
    chunk = g_heap.small.head;
    while (chunk)
    {
        if (chunk->head == ptr)
        {
            chunk->available = 1;
            chunk->freed = 1;
            return;
        }
        chunk = chunk->next;
    }
    chunk = g_heap.large;
    while (chunk)
    {
        if (chunk->head == ptr)
        {
            munmap((void *)chunk, chunk->size + sizeof(t_chunk));
            return;
        }
        chunk = chunk->next;
    } 
}