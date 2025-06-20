#ifndef MALLOC_H
# define MALLOC_H

# define TINY_ALLOC getpagesize() / 8
# define SMALL_ALLOC getpagesize()

# define TINY 0
# define SMALL 1
# define LARGE 2

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>

typedef struct s_chunk
{
    int available;
    unsigned int size;
    struct s_chunk *next;
    void *head;
    int freed;
    int type;
}               t_chunk;

typedef struct  s_page
{
    __uint128_t available;
    t_chunk *head;
    void    *heap;
}               t_page;

typedef struct s_heap
{
    t_page tiny;
    t_page small;
    t_chunk *large;
    bool initialized;
    // pthread_mutex_t mutex;
}               t_heap;



void *split_chunks(t_page *page, __uint32_t allocation);
void *big_allocation(size_t allocation_size, t_chunk **large);
void *sort_allocations(t_heap *heap, size_t size);
void ft_free(void *ptr);
void *ft_realloc(void *ptr, size_t size);

extern t_heap g_heap;

#endif