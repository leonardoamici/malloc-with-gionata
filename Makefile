# Compiler and flags
CC = gcc
FLAGS = -Wall -Werror -Wextra -fPIC

# System-specific variables
TYPE := $(shell uname -m)
SYS := $(shell uname -s)

ifeq ($(HOSTTYPE),)
    export HOSTTYPE := $(SYS)_$(TYPE)
endif

# Source and object files
SRCS = src/malloc.c src/realloc.c src/free.c src/attributes.c
OBJS = $(notdir $(SRCS:.c=.o))

# Output names
NAME = libft_malloc_$(HOSTTYPE).so
LNAME = libft_malloc.so
EXECUTABLE = main

# Default target: build shared lib + symlink
all: $(NAME)
	ln -sf $(NAME) $(LNAME)

# Build shared library
$(NAME): $(OBJS)
	$(CC) -shared -o $@ $^

# Compile .c files to .o in current dir
%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@

# Optional: build test executable
$(EXECUTABLE): $(OBJS) main.c
	$(CC) $(FLAGS) -o $@ $^ -L. -l:$(NAME)
	chmod +x $@

# Clean object files
clean:
	rm -f *.o

# Full clean: includes library and executable
fclean: clean
	rm -f $(NAME) $(LNAME) $(EXECUTABLE)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
