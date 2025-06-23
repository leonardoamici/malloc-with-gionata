# Compiler and flags
CC = gcc
FLAGS = -Wall -Werror -Wextra -fPIC

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
SRCS = src/malloc.c src/realloc.c src/free.c src/attributes.c
OBJS = $(notdir $(SRCS:.c=.o))

# Output names
NAME = libft_malloc_$(HOSTTYPE).$(SHARED_EXT)
LNAME = libft_malloc.$(SHARED_EXT)
EXECUTABLE = main

# Default target: build shared lib + symlink
all: libcomp $(NAME)
	ln -sf $(NAME) $(LNAME)

# Build shared library
$(NAME): $(OBJS)
	$(CC) $(SHARED_FLAG) -o $@ $^ $(LIBFT)

libcomp:
	@make -C Libft

# Compile .c files to .o in current dir
%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@

# Optional: build test executable
$(EXECUTABLE): $(OBJS) main.c
	$(CC) $(FLAGS) -o $@ $^ -L. -l:$(NAME)
	chmod +x $@

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

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
