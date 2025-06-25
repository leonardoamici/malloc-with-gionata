#include "../includes/libft_malloc_ubuntu.h"

t_heap g_heap;

t_chunk *last_chunk(t_page *page)
{
    t_chunk *temp = page->head;

    while (temp->next)
        temp = temp->next;

    return temp;
}

void *split_chunks(t_page *page, __uint32_t allocation)
{
    t_chunk *temp = page->head;
    // set the best chunk to the last one : TODO
    t_chunk *best = last_chunk(page);

    if (allocation == 0)
        allocation = 1;

    while (temp)
    {
        if (temp->size > allocation && temp->available)
        {
            best = temp;
            // printf("best set as %p", best);
        }
        temp = temp->next;
    }

    if (best->size < allocation)
        return (NULL);

    while (((long long int)best->head + allocation) % 16)
        allocation++;

    t_chunk *new;
    new = (void *)((char *)best->head + allocation);
    new->head = (void *)((char *)new + sizeof(t_chunk));
    new->size = best->size - (allocation + sizeof(t_chunk));
    new->next =  best->next;
    new->available = 1;
    new->type = best->type;

    best->available = 0;
    best->freed = 0;
    best->next = new;
    best->size = allocation;

    page->available -= allocation;
    return (best->head);
}

unsigned int print_memories(t_chunk *page, char *str)
{
    size_t allocation_size;

    allocation_size = 0;
    if (!page)
    {
        printf("%s : No memory allocated\n", str);
        return 0;
    }

    t_chunk *temp;
    temp = page;
    // unsigned int total_size = 0;

    printf("%s : %p\n", str, page);

    while (temp)
    {
        if (1)
        {
            printf("%p - %p : %d bytes%s\n", temp->head, temp->head + temp->size, temp->size, temp->freed ? " (free)" : "");
            allocation_size += temp->size;
        }
        temp = temp->next;
    }
    return allocation_size;
}

void *big_allocation(size_t allocation_size, t_chunk **large)
{
    int nbr_pages;
    void *mem = NULL;
    
    nbr_pages = allocation_size / sysconf(_SC_PAGESIZE) + 1;
    //if (allocation_size % getpagesize);
    //    nbr_pages++;
    
    mem = mmap(NULL, sysconf(_SC_PAGESIZE) * nbr_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    *large = (t_chunk *)mem;  // First chunk starts at the beginning of the mmap'd memory
    (*large)->head = (void *)((char *)mem + sizeof(t_chunk));  // Start after the chunk metadata
    (*large)->size = sysconf(_SC_PAGESIZE) * nbr_pages - sizeof(t_chunk);  // Remaining size after chunk metadata
    (*large)->next = NULL;
    (*large)->freed = 0;
    (*large)->available = 0;
    (*large)->type = LARGE;

    return ((*large)->head);
}

void *sort_allocations(t_heap *heap, size_t size)
{
    if (size > (size_t)SMALL_ALLOC)
        return (big_allocation(size, &heap->large));
    else if (size > (size_t)TINY_ALLOC)
        return (split_chunks(&heap->small, size));
    return (split_chunks(&heap->tiny, size));
}

int calculate_impaginations(int alloc_size)
{
    int needed;
    int impaginations;

    needed = (alloc_size + sizeof(t_chunk)) * 100;
    impaginations = needed / sysconf(_SC_PAGESIZE) + 1;

    return (impaginations);
}
 
void show_alloc_mem(void)
{
    unsigned int total_size = 0;

    total_size += print_memories(g_heap.tiny.head, "TINY");
    total_size += print_memories(g_heap.small.head, "SMALL");
    total_size += print_memories(g_heap.large, "LARGE");

    // Per Leo, il totale funziona, da rivedere questa soluzione però, al momemnto è una soluzione rapida e non mi convince molto, esiste sicuramente un modo migliore
    ft_printf("Total : %d bytes\n", total_size);
}

void *malloc(size_t size)
{
    void *new_alloc;

    pthread_mutex_lock(&g_heap.mutex);

    ft_printf("malloc called with size %d \n", size);

    new_alloc = sort_allocations(&g_heap, size);

    pthread_mutex_unlock(&g_heap.mutex);

    ft_printf("returned ptr %p \n", new_alloc);

    return (new_alloc);
}


/*int main()
{
    char *str = "string\n";

    char *str2 = malloc(10);

    int i = 0;

    for (int j = 0; j < 500; j++)
    {
        str2 = malloc(450);
    }
    if (str2 == NULL)
        return (0);
    write(1, "here\n", 5);
    while(str[i])
    {
        str2[i] = str[i];
        i++;
    }
    str2[i] = '\0';
    write(1, str2, 7);
}*/

//for large, anything bigger than medium. this isn't preallocated

//algo that splits chunks based on required memory 

//when memory is given it has to be aligned (check with % 16 == 0)

//another list that tracks the ammount of allocs and their size and their start (maybe for frees as well)