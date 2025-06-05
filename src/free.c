#include "../includes/malloc.h"

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

// static void unmap_large_chunk(t_chunk **head, t_chunk *chunk)
// {
//     t_chunk *current = *head;
//     t_chunk *prev = NULL;

//     while (current)
//     {
//         if (current == chunk)
//         {
//             if (prev)
//                 prev->next = current->next;
//             else
//                 *head = current->next;

//             munmap((void *)current, current->size + sizeof(t_chunk));
//             return;
//         }
//         current = current->next;
//     }
// }


void ft_free(void *ptr)
{
    if (!ptr)
    return;

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

    // current = g_heap.large;
    // while (current)
    // {
    //     if (current->head == ptr)
    //     {
    //         unmap_large_chunk(&g_heap.large, current);
    //         return;
    //     }
    //     current = current->next;
    // }
}