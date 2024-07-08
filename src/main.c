/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:30:09 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/28 15:30:09 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	input;
	int	output;

	if (envp[0] == NULL)
		return (1);
	if (argc < 4)
		return (2);
	input = open(argv[1], O_RDONLY);
	output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (input == -1 || output == -1)
	{
		if (input != -1)
			close(input);
		if (output != -1)
			close(output);
		return (3);
	}
	execute_cmds(argv + 1, envp, input, STDOUT_FILENO);
	return (0);
}
