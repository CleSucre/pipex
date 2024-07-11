/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:57:30 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 10:32:32 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUSTOM_TERMIOS_H
#define CUSTOM_TERMIOS_H

# include <unistd.h>
# include <stdio.h>
# include <termios.h>

typedef struct s_term
{
	struct termios original_termios;
} t_term;

void	enable_termios(t_term *term);
void	disable_termios(t_term *term);

#endif //CUSTOM_TERMIOS_H
