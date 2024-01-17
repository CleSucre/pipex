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

static void	child_process(int fdin, int fd[2], char **cmd, char **envp)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		close(fd[0]);
		dup2(fdin, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fdin);
		close(fd[1]);
		path = ft_get_path(cmd[0], envp);
		execve(path, cmd, envp);
		perror("execve");
		free(path);
		exit(1);
	}
	else
	{
		close(fdin);
		close(fd[1]);
		waitpid(pid, NULL, 0);
	}
}

static void	parent_process(int fdin, int fdou, char **cmd, char **envp)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return ;
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
		close(fdou);
		close(fdin);
		waitpid(pid, NULL, 0);
	}
}

static void	ft_exit_properly(char **cmd, int input, int output, int fd[2])
{
	ft_free(cmd);
	close(input);
	close(output);
	close(fd[0]);
	close(fd[1]);
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	int		output;
	int		fd[2];
	char	**cmd;

	if (envp[0] == NULL)
		return (1);
	if (argc != 5)
		return (2);
	if (pipe(fd) == -1)
		return (3);
	output = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	input = open(argv[1], O_RDONLY);
	if (input == -1 || output == -1)
		return (4);
	cmd = ft_split(argv[2], ' ');
	if (cmd == NULL)
		return (5);
	child_process(input, fd, cmd, envp);
	ft_free(cmd);
	cmd = ft_split(argv[3], ' ');
	if (cmd == NULL)
		return (6);
	parent_process(fd[0], output, cmd, envp);
	return (0);
}
