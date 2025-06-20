# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: leonardo <leonardo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/04 16:45:52 by leonardo          #+#    #+#              #
#    Updated: 2024/09/11 14:52:57 by leonardo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = gcc
FLAGS = -Wall -Werror -Wextra

# System-specific variables
TYPE := $(shell uname -m)
SYS := $(shell uname -s)

ifeq ($(HOSTTYPE),)
    export HOSTTYPE := $(SYS)_$(TYPE)
endif

# Output names
NAME = libft_malloc_$(HOSTTYPE).so
LNAME = libft_malloc.so
EXECUTABLE = main

# Source and object files
SRCS = src/malloc.c src/realloc.c src/free.c src/attributes.c
OBJS = $(SRCS:.c=.o)

SRCSM = main.c
OBJSM = $(SRCSM:.c=.o)

# Default rule (all targets)
all: $(NAME) $(EXECUTABLE)

# Compile object files and create shared library
$(NAME): $(OBJS) $(OBJSM)
	ln -sf $(LNAME) $(NAME)
	$(CC) -c -fPIC $(FLAGS) $(SRCS)
	$(CC) -shared -o $(LNAME) $(OBJS)

# Create executable linking against libft_malloc.so
$(EXECUTABLE):
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJSM) -L/. $(LNAME) -Wl,
	chmod +x $(EXECUTABLE)


# Clean up object files
clean:
	rm -rf *.o


# Full cleanup, including libraries and executables
fclean: clean
	rm -rf $(NAME) $(LNAME) $(EXECUTABLE)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all re clean fclean