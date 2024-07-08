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

	if (envp[0] == NULL)
		return (1);
	if (argc < 4)
		return (2);
	input = open(argv[1], O_RDONLY);
	if (input == -1)
	{
		close(input);
		return (3);
	}
	execute_cmds(argv + 1, envp, input, STDOUT_FILENO);
	return (0);
}
