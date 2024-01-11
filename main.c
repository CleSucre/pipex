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

//X_OK means executable

#include "pipex.h"

static void	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i++]);
	}
	free(str);
}

static void	exec_command(int fdin, char **cmd, char **envp, int fdou)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
	{
		return ;
	}
	else if (pid == 0)
	{
		path = ft_get_path(cmd[0], envp);
		dup2(fdin, STDIN_FILENO);
		dup2(fdou, STDOUT_FILENO);
		close(fdin);
		close(fdou);
		execve(path, cmd, envp);
		perror("execve");
		free(path);
		exit(1);
	}
	else
	{
		close(fdin);
		close(fdou);
		waitpid(pid, NULL, 0);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	int		output;
	int		fd[2];
	char	**cmd;

	if (envp[0] == NULL)
	{
		ft_printf("envp is empty\n");
		return (1);
	}
	if (argc == 5)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe creation");
			return (2);
		}
		input = open(argv[1], O_RDONLY);
		if (input == -1)
		{
			perror("input file");
			return (3);
		}
		cmd = ft_split(argv[2], ' ');
		if (cmd == NULL)
		{
			perror("split");
			return (5);
		}
		exec_command(input, cmd, envp, fd[1]);
		ft_free(cmd);
		cmd = ft_split(argv[3], ' ');
		if (cmd == NULL)
		{
			perror("split");
			return (6);
		}
		output = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output == -1)
		{
			perror("output file");
			return (4);
		}
		exec_command(fd[0], cmd, envp, output);
		ft_free(cmd);
	}
	return (0);
}
