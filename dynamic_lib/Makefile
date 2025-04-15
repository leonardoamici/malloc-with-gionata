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

LIBFT = Libft/libft.a

ifeq ($(HOSTTYPE),)
    export HOSTTYPE := $(SYS)_$(TYPE)
endif

# Output names
NAME = libft_malloc_$(HOSTTYPE).so
LNAME = libft_malloc.so
EXECUTABLE = main

# Source and object files
SRCS = malloc.c realloc.c free.c
OBJS = $(SRCS:.c=.o)

SRCSM = main.c
OBJSM = $(SRCSM:.c=.o)

# Default rule (all targets)
all: libcomp $(NAME) $(EXECUTABLE)

libcomp:
		@make -C Libft

# Compile object files and create shared library
$(NAME): $(OBJS) $(OBJSM)
	ln -sf $(LNAME) $(NAME)
	$(CC) -c -fPIC $(FLAGS) $(SRCS)
	$(CC) -shared -o $(LNAME) $(OBJS) $(LIBFT)

# Create executable linking against libft_malloc.so
$(EXECUTABLE):
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(OBJSM) $(LIBFT) -L/. $(LNAME) -Wl,-rpath=.
	chmod +x $(EXECUTABLE)

libclean:
	@make clean -C Libft

# Clean up object files
clean: libclean
	rm -rf *.o

libfclean:
		@make fclean -C Libft

# Full cleanup, including libraries and executables
fclean: libfclean clean
	rm -rf $(NAME) $(LNAME) $(EXECUTABLE)

# Rebuild the project
re: fclean all

# Phony targets
.PHONY: all re clean fclean