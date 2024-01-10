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

// TODO: rework this function
static char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**paths;

	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	path = ft_strdup(envp[i] + 5);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path, cmd);
		if (access(path, X_OK) == 0)
		{
			return (path);
		}
		free(path);
		i++;
	}
	ft_printf("command not found: %s\n", cmd);
	exit(1);
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
		path = get_path(cmd[0], envp);
		dup2(fdin, STDIN_FILENO);
		dup2(fdou, STDOUT_FILENO);
		execve(path, cmd, envp);
		free(path);
	}
	close(fdin);
	close(fdou);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	int		output;
	int		fd[2];
	char	**cmd;

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
			return (2);
		}
		output = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output == -1)
		{
			perror("output file");
			return (2);
		}
		cmd = ft_split(argv[2], ' ');
		if (cmd == NULL)
		{
			perror("split");
			return (2);
		}
		exec_command(input, cmd, envp, fd[1]);
		ft_free(cmd);
		cmd = ft_split(argv[3], ' ');
		if (cmd == NULL)
		{
			perror("split");
			return (2);
		}
		exec_command(fd[0], cmd, envp, output);
		ft_free(cmd);
		close(input);
		close(output);
	}
	return (0);
}
