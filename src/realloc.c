#include "../includes/malloc.h"

extern t_heap g_heap;

void    copy_mem(t_chunk *source, t_chunk *dest, size_t amount)
{
    int i;

    i = 0;
    while (i < amount)
    {
        dest[i] = source[i];
        i++;
    }
}

t_chunk *find_ptr(void *ptr, t_chunk *map)
{
    t_chunk *head;

    head = map;
    while (head)
    {
        if (head->head == ptr)
            return (head);
        head = head->next;
    }
    return (NULL);
}

t_chunk *check_available_alloc(void *ptr, size_t size)
{

    if (find_ptr(ptr, g_heap.tiny.head))
        return (find_ptr(ptr, g_heap.tiny.head));
    else if (find_ptr(ptr, g_heap.small.head))
        return (find_ptr(ptr, g_heap.small.head));
    else
        return (find_ptr(ptr, g_heap.large));
}

int sort_type(int size)
{
    if (size > SMALL_ALLOC)
        return (LARGE);
    else if (size > TINY_ALLOC)
        return (SMALL);
    return (TINY);
}

void *switch_allocation_type(t_chunk *alloc, size_t size)
{
    t_chunk *new_alloc = sort_allocations(&g_heap, size);

    printf("ciao mondo \n");

    copy_mem(alloc->head, new_alloc, size);
    ft_free(alloc->head);
    return (new_alloc->head);
}

void *resize_allocation(t_chunk *alloc, size_t size)
{
    if (alloc->next->available && (alloc->size + alloc->next->size > size))
    {
        alloc->next->head += size;
        alloc->next += size;
        alloc->next->size -= size - alloc->size;
        alloc->size = size;
        return (alloc->head);
    }
    else
        return (switch_allocation_type(alloc, size));
}

void *select_mapping(t_chunk *alloc, size_t size)
{
    if (alloc->type == sort_type(size))
        return(resize_allocation(alloc, size));
    else
        return (switch_allocation_type(alloc, size));
}

void *ft_realloc(void *ptr, size_t size)
{
    t_chunk *alloc;

    if (ptr == NULL)
        return(sort_allocations(&g_heap, size));

    alloc = check_available_alloc(ptr, size);
    if (alloc != NULL)
        return (select_mapping(alloc, size));
    return (alloc);
}