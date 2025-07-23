#include "../includes/libft_malloc_ubuntu.h"

extern t_heap g_heap;

static void merge_free_chunks(t_chunk **head)
{
    t_chunk *current = *head;

    while (current && current->next)
    {
        if (current->freed && current->next->freed)
        {
            current->size += sizeof(t_chunk) + current->next->size;
            current->next = current->next->next;
        }
        else
            current = current->next;
    }
}

static void unmap_large_chunk(t_chunk **head, t_chunk *chunk)
{
    t_chunk *current = *head;

    if (current == chunk)
    {
        munmap((void *)current, current->size + sizeof(t_chunk));
        g_heap.large = NULL;
        return;
    }
}


void sort_free(void *ptr)
{
    t_chunk *current = g_heap.tiny.head;
    while (current)
    {   
        if (current->head == ptr)
        {
            current->available = 1;
            current->freed = 1;
            merge_free_chunks(&g_heap.tiny.head);
            return;
        }
        current = current->next;
    }

    current = g_heap.small.head;
    while (current)
    {   
        if (current->head == ptr)
        {
            current->available = 1;
            current->freed = 1;
            merge_free_chunks(&g_heap.small.head);
            return;
        }
        current = current->next;
    }

    
    current = g_heap.large;
    t_chunk *prev = NULL;
    while (current->head != ptr)
    {
        prev = current;
        current = current->next;
        if (current->next == NULL)
            return ;
    }
    prev->next = current->next;
    unmap_large_chunk(&g_heap.large, current);
}

void free(void *ptr)
{
    if (!ptr)
        return;
    pthread_mutex_lock(&g_heap.mutex);

    ft_printf("free called on ptr %p\n", ptr);
    //ft_printf("free called on ptr %p\n", ptr);
    sort_free(ptr);
    
    pthread_mutex_unlock(&g_heap.mutex);
    //ft_printf("free exiting\n");
}