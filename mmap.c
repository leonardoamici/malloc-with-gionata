#include <sys/mman.h>
#include <unistd.h>
#include "malloc.h"

void *test()
{
    void *addr  = NULL;
    char *test = NULL;
    int *test2 = NULL;
    addr = mmap(NULL, getpagesize() * 13, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("%d\n", getpagesize());
    test = (char *)addr;
    test2 = (int *)addr + sizeof(char *);
    return ((void *)test2);
}

void init_allocation()
{
    void *addr_tiny;
    void *addr_small;

    addr_tiny = mmap(NULL, getpagesize() * 14, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    addr_small = mmap(NULL, getpagesize() * 101, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}


int main()
{
    //printf("%ld\n", sizeof(t_chunk));
    while (1)
    {
        mmap(NULL, getpagesize() * 10, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        sleep(5);
    }
    return (0);
}

//for small 14 = (512 + 24) * 100 -> 13.08| 112

//for medium 100 = 40960 |  101

//for large, anything bigger than medium. this isn't preallocated

//algo that splits chunks based on required memory 

//when memory is given it has to be aligned (check with % 16 == 0)

//another list that tracks the ammount of allocs and their size and their start (maybe for frees as well)