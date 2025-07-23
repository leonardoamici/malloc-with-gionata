#include "../includes/libft_malloc_ubuntu.h"

extern t_heap g_heap;

void print_size_t(size_t n) 
{
    char buffer[32];
    int i = 30;

    if (n == 0)
    {
        buffer[i--] = '0';
    }

    else 
    {
        while (n > 0) 
        {
            buffer[i--] = '0' + (n % 10);
            n /= 10;
        }
    }

    write(1, &buffer[i + 1], 31 - i);
}

t_chunk *find_ptr(void *ptr, t_chunk *map)
{
    t_chunk *head;

    head = map;
    while (head)
    {
        if (head->head == ptr)
        {
            return (head);
        }
        head = head->next;
    }
    return (NULL);
}

void *check_available_alloc(void *ptr)
{

    if (find_ptr(ptr, g_heap.tiny.head))
    {
        return (find_ptr(ptr, g_heap.tiny.head));
    }
    else if (find_ptr(ptr, g_heap.small.head))
    {
        return (find_ptr(ptr, g_heap.small.head));
    }
    else
    {
        return (find_ptr(ptr, g_heap.large));
    }
}

size_t align16(size_t size)
{
    return (size + 15) & ~((size_t)15);
}