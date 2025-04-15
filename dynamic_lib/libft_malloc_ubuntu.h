/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_malloc_ubuntu.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonardo <leonardo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:48:50 by leonardo          #+#    #+#             */
/*   Updated: 2024/09/30 16:41:27 by leonardo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include "Libft/libft.h"
    
typedef struct  s_chunk
{
    int size; 
    bool utilized;
    struct s_chunk *next;
    void *start;
}               t_chunk;

typedef struct  s_page
{
    __uint32_t available;
    t_chunk *head;
    void    *heap;
}               t_page;

typedef struct  s_malloc_page
{
    t_page  *tiny;
    t_page  *small;
    t_chunk *large;
}               t_malloc_page;

void  free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif