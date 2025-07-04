#include "../includes/libft_malloc_ubuntu.h"

extern t_heap g_heap;

void    copy_mem(void *source, void *dest, size_t amount)
{
    size_t i;

    i = 0;
    while (i < amount)
    {
        ((char *)dest)[i] = ((char *)source)[i];
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

t_chunk *check_available_alloc(void *ptr)
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
    void *new_alloc = sort_allocations(&g_heap, size);

    copy_mem(alloc->head, new_alloc, size);
    free(alloc->head);
    return (new_alloc);
}

void *resize_allocation(t_chunk *alloc, size_t size)
{
    if (alloc->next && alloc->next->available && (alloc->size + sizeof(t_chunk) + alloc->next->size >= size))
    {
        // How much we can take from next chunk
        size_t total_size = alloc->size + sizeof(t_chunk) + alloc->next->size;
        size_t remaining = total_size - size;

        if (remaining > sizeof(t_chunk) + 1) {
            // Shrink next chunk and shift its pointer
            t_chunk *new_next = (t_chunk *)((char *)alloc->head + size);
            new_next->head = (void *)((char *)new_next + sizeof(t_chunk));
            new_next->size = remaining - sizeof(t_chunk);
            new_next->available = 1;
            new_next->next = alloc->next->next;

            alloc->size = size;
            alloc->next = new_next;
        } 
        /*else {
            // Not enough space for new chunk, consume all
            alloc->size = total_size;
            alloc->next = alloc->next->next;
        }*/
        return alloc->head;
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

void *realloc(void *ptr, size_t size)
{
    t_chunk *alloc;


    pthread_mutex_lock(&g_heap.mutex);

    printf("realloc called on ptr %p with size %zu\n", ptr, size);

    if (ptr == NULL)
    {
        pthread_mutex_unlock(&g_heap.mutex);
        return(sort_allocations(&g_heap, size));
    }

    alloc = check_available_alloc(ptr);
    if (alloc != NULL)
    {
        pthread_mutex_unlock(&g_heap.mutex);
        return (select_mapping(alloc, size));
    }
    pthread_mutex_unlock(&g_heap.mutex);
    return (alloc);

}