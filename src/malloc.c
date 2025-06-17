#include "../includes/malloc.h"

t_heap g_heap;


void init_allocation(t_page *pages, int size, int type)
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
    pages->head->available = 1;
    pages->head->type = type;
}

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
        if (!temp->available)
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
    (*large)->type = LARGE;

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
    unsigned int total_size = 0;

    total_size += print_memories(heap->tiny.head, "TINY");
    total_size += print_memories(heap->small.head, "SMALL");
    total_size += print_memories(heap->large, "LARGE");

    // Per Leo, il totale funziona, da rivedere questa soluzione però, al momemnto è una soluzione rapida e non mi convince molto, esiste sicuramente un modo migliore
    printf("Total : %d bytes\n", total_size);
}

void *ft_malloc(size_t size)
{
    if (g_heap.initialized == false)
    {
        init_allocation(&g_heap.tiny, calculate_impaginations(TINY_ALLOC), TINY);
        init_allocation(&g_heap.small, calculate_impaginations(SMALL_ALLOC), SMALL);
        g_heap.initialized = true;
    }
    return sort_allocations(&g_heap, size);
}


int main()
{
    void *a = ft_malloc(1);
    void *b = ft_malloc(120);
    void *c = ft_malloc(100);
    void *d = ft_malloc(1000);
    void *e = ft_malloc(5000);
    // void *c = ft_malloc(130);

    show_alloc_mem(&g_heap);

    printf("\nAfter realloc 1:\n\n");
    ft_free(c);
    a = ft_realloc(a, 5000);
    show_alloc_mem(&g_heap);
    
    
    printf("\nAfter realloc 2:\n\n");
    e = ft_realloc(e, 10);
    show_alloc_mem(&g_heap);
    

}

//for large, anything bigger than medium. this isn't preallocated

//algo that splits chunks based on required memory 

//when memory is given it has to be aligned (check with % 16 == 0)

//another list that tracks the ammount of allocs and their size and their start (maybe for frees as well)