#include "../includes/libft_malloc_ubuntu.h"

t_heap g_heap;

unsigned int print_memories(t_chunk *page, char *str)
{
    size_t allocation_size;

    allocation_size = 0;
    if (!page)
    {
        ft_printf("%s : No memory allocated\n", str);
        return 0;
    }

    t_chunk *temp;
    temp = page;

    ft_printf("%s : %p\n", str, page);

    while (temp)
    {
        if (!temp->freed)
        {
            ft_printf("%p - %p : ", temp->head, (void *)((char *)temp->head + temp->size));
            print_size_t(temp->size);
            ft_printf(" bytes \n");
            allocation_size += temp->size;
        }
        temp = temp->next;
    }
    return allocation_size;
}

unsigned int print_memories_ex(t_chunk *page, char *str)
{
    size_t allocation_size;

    allocation_size = 0;
    if (!page)
    {
        ft_printf("%s : No memory allocated\n", str);
        return 0;
    }

    t_chunk *temp;
    temp = page;

    ft_printf("%s : %p\n", str, page);

    while (temp)
    {
        ft_printf("%p - %p : ", temp->head, (void *)((char *)temp->head + temp->size));
        print_size_t(temp->size);
        ft_printf(" bytes %s\n", temp->freed ? " (free)" : "");
        allocation_size += temp->size;
        temp = temp->next;
    }
    return allocation_size;
}

void print_hexa_dump(void *ptr)
{
    t_chunk *temp;
    size_t i;

    i = 0;
    temp = check_available_alloc(ptr);

    while (i < temp->size)
    {
        ft_printf("%p ", (void *)((char *)ptr + (i)));
        i++;
        while (i % 16 != 0)
        {
            if(!(int)*((char *)ptr + (i)))
                write(1, " ", 1);
            ft_printf("%x ", (int)*((char *)ptr + (i)));
            i++;
        }
        ft_printf("\n");
    }
}

void show_alloc_mem(void)
{
    unsigned int total_size = 0;

    total_size += print_memories(g_heap.tiny.head, "TINY");
    total_size += print_memories(g_heap.small.head, "SMALL");
    total_size += print_memories(g_heap.large, "LARGE");

    ft_printf("Total : %d bytes\n", total_size);
}

void show_alloc_mem_ex(void)
{
    unsigned int total_size = 0;

    total_size += print_memories_ex(g_heap.tiny.head, "TINY");
    total_size += print_memories_ex(g_heap.small.head, "SMALL");
    total_size += print_memories_ex(g_heap.large, "LARGE");

    ft_printf("Total : %d bytes\n", total_size);
}