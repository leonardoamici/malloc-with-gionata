#include "../includes/libft_malloc_ubuntu.h"

extern t_heap g_heap;

int calculate_impaginations(size_t alloc_size)
{
    int needed;
    int impaginations;

    needed = (alloc_size + sizeof(t_chunk)) * 500;
    impaginations = needed / sysconf(_SC_PAGESIZE) + 1;

    return (impaginations);
}

void init_allocation(t_page *pages, int size, int type)
{
    void *mem = NULL;

    mem = mmap(NULL, sysconf(_SC_PAGESIZE) * size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        write(2, "failed to map memory\n", 22);
        exit(1);
    }
    pages->heap = (t_page*)mem;
    pages->head = (t_chunk *)(pages)->heap;
    pages->head->head = (void *)((char *)(pages)->heap + sizeof(t_chunk));
    pages->head->size = sysconf(_SC_PAGESIZE) * size - sizeof(t_chunk);
    pages->head->next = NULL;
    pages->head->freed = 0;
    pages->head->available = 1;
    pages->head->type = type;
}

void __attribute__((constructor, used)) initialize_heap() {
    if (g_heap.initialized)
        return;
    init_allocation(&g_heap.tiny, calculate_impaginations(TINY_ALLOC), TINY);
    init_allocation(&g_heap.small, calculate_impaginations(SMALL_ALLOC), SMALL);
    pthread_mutex_init(&g_heap.mutex, NULL);
    g_heap.large = NULL;
    g_heap.initialized = true;
}

void __attribute__((destructor, used)) destroy_heap() {

    munmap(g_heap.tiny.heap, TINY_ALLOC);
    munmap(g_heap.small.heap, SMALL_ALLOC);

    void *temp;

    while (g_heap.large)
    {
        temp = (void *)g_heap.large->next;
        munmap(g_heap.large->head, g_heap.large->size + sizeof(t_chunk));
        g_heap.large = (t_chunk *)temp;
    }
}