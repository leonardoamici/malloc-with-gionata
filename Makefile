# Compiler and Cflags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC -g

LIBFT = Libft/libft.a

# System-specific variables
TYPE := $(shell uname -m)
SYS := $(shell uname -s)

ifeq ($(HOSTTYPE),)
    export HOSTTYPE := $(SYS)_$(TYPE)
endif

# Platform-dependent extension
ifeq ($(SYS),Darwin)
    SHARED_EXT = dylib
    SHARED_FLAG = -dynamiclib
else
    SHARED_EXT = so
    SHARED_FLAG = -shared
endif

# Source and object files
SRCS = src/malloc.c src/realloc.c src/free.c src/attributes.c src/show_alloc_mem.c src/utils.c
OBJS = $(notdir $(SRCS:.c=.o))

# Output names
NAME = libft_malloc_$(HOSTTYPE).$(SHARED_EXT)
LNAME = libft_malloc.$(SHARED_EXT)
EXECUTABLE = main

# Default target: build shared lib + symlink
all: libcomp $(NAME) $(EXECUTABLE)


# Build shared library
$(NAME): $(OBJS)
	$(CC) -g -w -Wno-unused-result $(SHARED_FLAG) -o $@ $^ $(LIBFT)
	ln -sf $(NAME) $(LNAME)

libcomp:
	@make -C Libft

# Compile .c files to .o in current dir
%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Optional: build test executable
$(EXECUTABLE): $(OBJS)
	$(CC) -o main.test ./main.c ./$(LNAME)

libclean:
	@make clean -C Libft

# Clean object files
clean: libclean
	rm -f *.o

libfclean:
	@make fclean -C Libft

# Full clean: includes library and executable
fclean: libfclean clean
	rm -f $(NAME) $(LNAME) $(EXECUTABLE)
	rm -f *.test

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re 
