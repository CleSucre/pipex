/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termios.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:57:09 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 10:07:46 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "custom_termios.h"

/**
 * @brief Save the original termios settings
 *
 * @param t_term *term
 * @return void
 */
static void save_termios(t_term *term)
{
	if (tcgetattr(STDIN_FILENO, &term->original_termios) == -1)
		perror("tcgetattr");
}

/**
 * @brief Enable termios to read char, and catch exhaust sequence
 *
 * @param t_term *term
 * @return void
 */
void enable_termios(t_term *term)
{
	struct termios new_termios;

	save_termios(term);
	new_termios = term->original_termios;
	new_termios.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_termios.c_cc[VERASE] = 127;
	new_termios.c_iflag &= ~(ICRNL | INLCR | IGNCR);
	new_termios.c_cc[VMIN] = 1;
	new_termios.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_termios) == -1)
		perror("tcsetattr");
}

/**
 * @brief Restore default settings of termios
 *
 * @param t_term *termios
 * @return void
 */
void disable_termios(t_term *term)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term->original_termios) == -1)
		perror("tcsetattr");
}
