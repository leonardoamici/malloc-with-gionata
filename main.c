#include "includes/libft_malloc_ubuntu.h"

int main(int ac, char **av, char **envp)
{
    execve("/bin/clear", (char **){NULL}, envp);
    return 0;
}