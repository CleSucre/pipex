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

static int	ft_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return (1);
}

static char	*ft_get_path_n(char **cmd, char **envp)
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
			ft_free(paths);
			return (path);
		}
		free(path);
	}
	ft_free(cmd);
	exit(ft_free(paths));
}

void	ft_exec_cmd(int fdin, int fdou, char **cmd, char **envp)
{
	int		pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		path = ft_get_path_n(cmd, envp);
		dup2(fdin, STDIN_FILENO);
		dup2(fdou, STDOUT_FILENO);
		close(fdin);
		close(fdou);
		execve(path, cmd, envp);
		perror("execve");
		ft_free(cmd);
		free(path);
		exit(1);
	}
	close(fdou);
	close(fdin);
	waitpid(pid, NULL, 0);
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
static void	ft_execute_shell(int input, int output, char **cmds, char **envp)
{
	int		i;
	int		fd[2];
	char	**cmd;
	size_t	cmds_len;

	cmds_len = ft_strlentab((const char **) cmds);
	if (cmds_len < 3)
		return ;
	i = 1;
	while (i < cmds_len - 2)
	{
		pipe(fd);
		cmd = ft_split(cmds[i], " ");
		ft_exec_cmd(input, fd[1], cmd, envp);
		input = fd[0];
		ft_free(cmd);
		i++;
	}
	cmd = ft_split(cmds[i], " ");
	ft_exec_cmd(input, output, cmd, envp);
	ft_free(cmd);
	close(fd[0]);
	close(fd[1]);
}

int	main(int argc, char **argv, char **envp)
{
	int		input;
	int		output;

	if (envp[0] == NULL)
		return (1);
	if (argc < 4)
		return (2);
	input = open(argv[1], O_RDONLY);
	output = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (input == -1 || output == -1)
		return (3);
	ft_execute_shell(input, output, argv + 1, envp);
	close(input);
	close(output);
	return (0);
}
