/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/08 11:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Close the pipe properly depending on the number of commands
 *
 * @param int cmds_len
 * @param int fd[2]
 * @param int input
 * @param int output
 */
void	close_pipe(int cmds_len, int fd[2], int input, int output)
{
	if (cmds_len < 3)
	{
		close(fd[0]);
		close(fd[1]);
	}
	close(input);
	close(output);
}

/**
 * @brief Execute the command
 *
 * @param int io[2]
 * @param char **cmds
 * @param char **envp
 * @param int toclose
 */
static void	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	int		pid;

	pid = fork();
	if (pid < 0)
		return ;
	else if (pid == 0)
	{
		dup2(cmd->input, STDIN_FILENO);
		dup2(cmd->output, STDOUT_FILENO);
		close(cmd->input);
		close(cmd->output);
		if (cmd->toclose != -1)
			close(cmd->toclose);
		if (execve(cmd->path, cmd->args, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, NULL, 0);
}

/**
 * Execute cmds in the shell and redirect
 * the output of each command to the
 * input of the next one using pipes given in parameter
 *
 * @param fd[2]		pipe files descriptors to communicate
 * 	input & output between each process
 * @param io[0]		file descriptor from the input file
 * @param io[1]		file descriptor from the output file
 * @param cmds		commands to execute,
 * 		cmds[0] ---> intput file
 * 		cmds[1] ---> first command to execute
 * 		cmds[2] ---> second command to execute
 * 		...
 * 		cmds[n] ---> output file
 * @param envp		environment variables
 */
void	execute_cmds(char **cmds, char **envp, int input, int output)
{
	int		i;
	int		fd[2];
	t_cmd	*cmd;
	size_t	cmds_count;

	cmds_count = ft_strlentab((const char **)cmds);
	i = 1;
	while (i < cmds_count - 2)
	{
		pipe(fd);
		output = fd[1];
		cmd = load_command(cmds[i], envp, input, output);
		if (cmd)
		{
			cmd->toclose = fd[0];
			ft_exec_cmd(cmd, envp);
			destroy_cmd(cmd);
			close(fd[1]);
		}
		close(input);
		input = fd[0];
		i++;
	}
	output = open(cmds[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd = load_command(cmds[i], envp, input, output);
	if (!cmd)
	{
		close(input);
		close(output);
		return ;
	}
	cmd->toclose = fd[1];
	if (cmds_count > 3)
		ft_exec_cmd(cmd, envp);
	else
	{
		cmd->toclose = -1;
		ft_exec_cmd(cmd, envp);
	}
	destroy_cmd(cmd);
	close_pipe(cmds_count, fd, input, output);
}
