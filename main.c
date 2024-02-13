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

// X_OK means executable

#include "pipex.h"

static char	*ft_get_path(char **cmd, char **envp, int io[2], int toclose)
{
	int		i;
	char	*path;
	char	**paths;
	char	*tmp;

	if (access(cmd[0], X_OK) == 0)
		return (cmd[0]);
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	paths = ft_split(envp[i] + 5, ":");
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(path, X_OK) == 0)
		{
			ft_freetab(paths);
			return (path);
		}
		free(path);
	}
	ft_freetab(cmd);
	ft_freetab(paths);
	close(io[0]);
	close(io[1]);
	if (toclose != -1)
		close(toclose);
	exit(1);
}

void	ft_exec_cmd(int io[2], char **cmd, char **envp, int toclose)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		path = ft_get_path(cmd, envp, io, toclose);
		dup2(io[0], STDIN_FILENO);
		dup2(io[1], STDOUT_FILENO);
		close(io[0]);
		close(io[1]);
		if (toclose != -1)
			close(toclose);
		execve(path, cmd, envp);
		perror("execve");
		ft_freetab(cmd);
		free(path);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	ft_freetab(cmd);
}

/**
 * Execute cmds in the shell and redirect
 * the output of each command to the
 * input of the next one using pipes given in parameter
 *
 * @param fd[2]		pipe files descriptors to communicate
 * 	input & output between each process
 * @param input		file descriptor from the input file
 * @param output	file descriptor from the output file
 * @param cmds		commands to execute,
 * 		cmds[0] ---> intput file
 * 		cmds[1] ---> first command to execute
 * 		cmds[2] ---> second command to execute
 * 		...
 * 		cmds[n] ---> output file
 * @param envp		environment variables
 */

static void	ft_execute_shell(int io[2], char **cmds, char **envp)
{
	int		i;
	int		fd[2];
	char	**cmd;
	size_t	cmds_len;

	cmds_len = ft_strlentab((const char **)cmds);
	i = 1;
	while (i < cmds_len - 2)
	{
		pipe(fd);
		cmd = ft_split(cmds[i], " ");
		io[1] = fd[1];
		ft_exec_cmd(io, cmd, envp, fd[0]);
		close(fd[1]);
		close(io[0]);
		close(io[0]);
		io[0] = fd[0];
		i++;
	}
	io[1] = open(cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd = ft_split(cmds[i], " ");
	ft_printf("cmds_len: %d\n", cmds_len);
	if (cmds_len > 3)
		ft_exec_cmd(io, cmd, envp, fd[1]);
	else
		ft_exec_cmd(io, cmd, envp, -1);
	close(io[0]);
	close(io[1]);
	if (cmds_len < 3)
	{
		close(fd[0]);
		close(fd[1]);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	io[2];

	if (envp[0] == NULL)
		return (1);
	if (argc < 4)
		return (2);
	io[0] = open(argv[1], O_RDONLY);
	io[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (io[0] == -1 || io[1] == -1)
	{
		if (io[0] != -1)
			close(io[0]);
		if (io[1] != -1)
			close(io[1]);
		return (3);
	}
	close(io[1]);
	ft_execute_shell(io, argv + 1, envp);
	return (0);
}
