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

    copy_mem(alloc->head, new_alloc,  alloc->size);
    //free(alloc->head);
    return (new_alloc);
}
void *resize_allocation(t_chunk *alloc, size_t size)
{

    if (alloc->next && alloc->next->available && (alloc->size + sizeof(t_chunk) + alloc->next->size >= size))
    {
        size_t total_size = alloc->size + sizeof(t_chunk) + alloc->next->size;
        size_t remaining = total_size - size;

        if (remaining > sizeof(t_chunk) + 1) {
            t_chunk *new_next = (t_chunk *)((char *)alloc->head + size);
            new_next->head = (void *)((char *)new_next + sizeof(t_chunk));
            new_next->size = remaining - sizeof(t_chunk);
            new_next->available = 1;
            new_next->next = alloc->next->next;

            alloc->size = size;
            alloc->next = new_next;
        } 
        /*else {
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
    {
        return(resize_allocation(alloc, size));
    }
    else
    {
        return (switch_allocation_type(alloc, size));
    }
}

void *realloc(void *ptr, size_t size)
{
    t_chunk *alloc;
    void *new_mem;

    new_mem = NULL;
    pthread_mutex_lock(&g_heap.mutex);

    size = align16(size);
    //ft_printf_fd(2, "realloc((void *)%p, %d)\n", ptr, size);
    //ft_printf("realloc called on %p \n", ptr);
    if (ptr == NULL)
    {
        new_mem = sort_allocations(&g_heap, size);
        pthread_mutex_unlock(&g_heap.mutex);
        return(new_mem);
    }

    alloc = check_available_alloc(ptr);
    
    if (alloc != NULL && alloc->size != size)
    {
        new_mem = select_mapping(alloc, size);
        pthread_mutex_unlock(&g_heap.mutex);
        if (alloc->head != new_mem)
            free(alloc->head);
        return (new_mem);
    }
    pthread_mutex_unlock(&g_heap.mutex);
    return (alloc);
}