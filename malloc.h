#ifndef MALLOC_H
# define MALLOC_H

typedef struct s_chunk
{
    int available;
    unsigned int size;
    struct s_chunk *next;
    void *head;
    int freed;
}               t_chunk;

typedef struct s_heap
{
    t_chunk *tiny;
    t_chunk *small;
    t_chunk *large;
}               t_heap;

#endif