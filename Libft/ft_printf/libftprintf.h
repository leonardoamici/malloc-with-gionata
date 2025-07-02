/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 09:09:05 by lamici            #+#    #+#             */
/*   Updated: 2023/03/08 11:40:58 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H
# include <stdarg.h>
# include <unistd.h>

int		ft_char(int c);
int		ft_digit(int n);
int		ft_hexd(unsigned int n, char format);
int		ft_path(unsigned long int n);
int		ft_printf(const char *str, ...);
int		ft_string(char *str);
int		ft_unsigned(long long unsigned n);
int		ft_matrix(char **mat);
int     size_t_to_str(size_t value, char *buf);

#endif
