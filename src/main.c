/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:30:09 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 12:22:11 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Split commands by pipe and trim whitespaces
 *
 * @param char *str
 * @return char **
 */
static char **split_cmds(char *str)
{
	char	**cmds;
	char	*trimmed;
	int		i;

	cmds = ft_split(str, "|");
	i = 0;
	while (cmds[i])
	{
		trimmed = ft_strtrim(cmds[i], WHITESPACES);
		free(cmds[i]);
		cmds[i] = trimmed;
		i++;
	}
	return (cmds);
}

/**
 * @brief Start listening for input, execute commands when a line is received.
 *
 * @param char	**envp
 * @param int	input
 * @param int	output
 * @return		int
 */
static int listen_input(char **envp, int input, int output, t_term *term)
{
	char	*cmd;
	char	**cmds;
	char	c;
	ssize_t	bits;

	cmd = ft_calloc(1, sizeof(char));
	if (cmd == NULL)
		return (1);
	cmd[0] = '\0';
	while (1)
	{
		bits = read(input, &c, 1);
		if (bits == -1)
		{
			free(cmd);
			return (1);
		}
		if (c == 13 || c == '\n')
		{
			write(STDOUT_FILENO, "\n", 1);
			cmds = split_cmds(cmd);
			execute_cmds(cmds, envp, input, output, term);
			ft_freetab(cmds);
			free(cmd);
			cmd = ft_calloc(1, sizeof(char));
			if (cmd == NULL)
				return (1);
			cmd[0] = '\0';
		}
		else if (c == 4 || c == 3)
			break ;
		else
		{
			cmd = ft_charjoin(cmd, c);
			if (cmd == NULL)
				return (1);
			write(STDOUT_FILENO, &c, 1);
		}
	}
	free(cmd);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	int		output;
	t_term	term;

	(void)argc;
	if (envp[0] == NULL)
		return (1);
	if (argc >= 2)
		input = open(argv[1], O_RDONLY);
	else
		input = STDIN_FILENO;
	output = STDOUT_FILENO;
	enable_termios(&term);
	listen_input(envp, input, output, &term);
	disable_termios(&term);
	if (input != STDIN_FILENO)
		close(input);
	return (0);
}
