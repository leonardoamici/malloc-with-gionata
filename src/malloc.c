#include "../includes/malloc.h"

t_heap g_heap;

/*void *test()
{
    void *addr  = NULL;
    char *test = NULL;
    int *test2 = NULL;
    addr = mmap(NULL, getpagesize() * 13, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("%d\n", getpagesize());
    test = (char *)addr;
    test2 = (int *)addr + sizeof(char *);
    return ((void *)test2);
}*/

void init_allocation(t_page *pages, int size)
{
    void *mem = NULL;

    mem = mmap(NULL, getpagesize() * size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //printf("%p pages inner hellos 1 \n", pages);
    pages->heap = (t_page*)mem;
    //printf("inner hellos 2 \n");
    //pages->small = mmap(NULL, getpagesize() * 101, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    pages->head = (t_chunk *)(pages)->heap;  // First chunk starts at the beginning of the mmap'd memory
    pages->head->head = (void *)((char *)(pages)->heap + sizeof(t_chunk));  // Start after the chunk metadata
    pages->head->size = getpagesize() * size - sizeof(t_chunk);  // Remaining size after chunk metadata
    //printf("size is %d\npagesize is %d \nnbr of pages is %d\n", pages->head->size, getpagesize(), size);
    //printf("inner hellos 3 \n");
    pages->head->next = NULL;
    pages->head->freed = 0;
    pages->head->available = 0;
}

void *split_chunks(t_page *page, __uint32_t allocation)
{
    t_chunk *temp = page->head;
    t_chunk *best = temp;

    while (temp)
    {
        if (temp->size > allocation)
            best = temp;
        temp = temp->next;
    }

    while (((long long int)best->head + allocation) % 16)
        allocation++;

    t_chunk *new;
    new = (void *)((char *)best->head + allocation);
    new->head = (void *)((char *)new + sizeof(t_chunk));
    new->size = best->size - (allocation + sizeof(t_chunk));
    new->next = NULL;
    new->available = 1;

    best->available = 0;
    best->next = new;
    best->size = allocation;

    page->available -= allocation;
    return (best->head);
}

int print_memories(t_page *page, char *str)
{
    t_chunk *temp;
    int size = 0;

    temp = page->head;
    printf("%s - %p\n", str, page->head);
    while (temp)
    {
        printf("%p - %p : %d bytes\n", temp->head, temp->head + temp->size, temp->size);
        size += temp->size;
        temp = temp->next;
    }
    return (size);
}

void *big_allocation(size_t allocation_size, t_chunk **large)
{
    int nbr_pages;
    void *mem = NULL;
    
    nbr_pages = allocation_size / getpagesize() + 1;
    //if (allocation_size % getpagesize);
    //    nbr_pages++;
    
    mem = mmap(NULL, getpagesize() * nbr_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    *large = (t_chunk *)mem;  // First chunk starts at the beginning of the mmap'd memory
    (*large)->head = (void *)((char *)mem + sizeof(t_chunk));  // Start after the chunk metadata
    (*large)->size = getpagesize() * nbr_pages - sizeof(t_chunk);  // Remaining size after chunk metadata
    (*large)->next = NULL;
    (*large)->freed = 0;
    (*large)->available = 0;

    return ((*large)->head);
}

void *sort_allocations(t_heap *heap, size_t size)
{
    if (size > SMALL_ALLOC)
        return (big_allocation(size, &heap->large));
    else if (size > TINY_ALLOC)
        return (split_chunks(&heap->small, size));
    return (split_chunks(&heap->tiny, size));
}

int calculate_impaginations(int alloc_size)
{
    int needed;
    int impaginations;

    needed = (alloc_size + sizeof(t_chunk)) * 100;
    impaginations = needed / getpagesize() + 1;

    return (impaginations);
}

void show_alloc_mem(t_heap *heap)
{
    print_memories(&heap->tiny, "TINY");
    print_memories(&heap->small, "SMALL");
}

void *ft_malloc(size_t size)
{
    if (g_heap.initialized == false)
    {
        init_allocation(&g_heap.tiny, calculate_impaginations(TINY_ALLOC));
        init_allocation(&g_heap.small, calculate_impaginations(SMALL_ALLOC));
        g_heap.initialized = true;
    }
    //printf("hellos\n");
    //sort_allocations(&heap, 17000);
    // sort_allocations(&g_heap, size);
    // sort_allocations(&g_heap, 100);
    // sort_allocations(&g_heap, 600);
    // show_alloc_mem(&g_heap);
    return sort_allocations(&g_heap, size);
}


int main()
{
    void *a = ft_malloc(10);

    show_alloc_mem(&g_heap);

    ft_free(a);

    printf ("after free\n");
    show_alloc_mem(&g_heap);
}

//for small 14 = (512 + 24) * 100 -> 13.08| 112

//for medium 100 = 40960 |  101

//for large, anything bigger than medium. this isn't preallocated

//algo that splits chunks based on required memory 

//when memory is given it has to be aligned (check with % 16 == 0)

//another list that tracks the ammount of allocs and their size and their start (maybe for frees as well)