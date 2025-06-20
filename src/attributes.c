#include "../includes/libft_malloc_ubuntu.h"

extern t_heap g_heap;

void init_allocation(t_page *pages, int size, int type)
{
    void *mem = NULL;

    mem = mmap(NULL, sysconf(_SC_PAGESIZE) * size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    //printf("%p pages inner hellos 1 \n", pages);
    pages->heap = (t_page*)mem;
    //printf("inner hellos 2 \n");
    //pages->small = mmap(NULL, sysconf(_SC_PAGESIZE) * 101, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    pages->head = (t_chunk *)(pages)->heap;  // First chunk starts at the beginning of the mmap'd memory
    pages->head->head = (void *)((char *)(pages)->heap + sizeof(t_chunk));  // Start after the chunk metadata
    pages->head->size = sysconf(_SC_PAGESIZE) * size - sizeof(t_chunk);  // Remaining size after chunk metadata
    //printf("size is %d\npagesize is %d \nnbr of pages is %d\n", pages->head->size, sysconf(_SC_PAGESIZE), size);
    //printf("inner hellos 3 \n");
    pages->head->next = NULL;
    pages->head->freed = 0;
    pages->head->available = 1;
    pages->head->type = type;
}

void __attribute__((constructor)) initialize_heap() {
    init_allocation(&g_heap.tiny, calculate_impaginations(TINY_ALLOC), TINY);
    init_allocation(&g_heap.small, calculate_impaginations(SMALL_ALLOC), SMALL);
    pthread_mutex_init(&g_heap.mutex, NULL);
    g_heap.initialized = true;
}