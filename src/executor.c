/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 12:23:07 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Execute the command
 *
 * @param char **cmds
 * @param char **envp
 */
static void ft_exec_cmd(t_cmd *cmd, char **envp, t_term *term)
{
	disable_termios(term);
	pid_t pid = fork();
	if (pid == 0)
	{
		if (cmd->input != STDIN_FILENO)
		{
			dup2(cmd->input, STDIN_FILENO);
			close(cmd->input);
		}
		if (cmd->output != STDOUT_FILENO)
		{
			dup2(cmd->output, STDOUT_FILENO);
			close(cmd->output);
		}
		if (cmd->toclose != -1)
			close(cmd->toclose);
		execve(cmd->path, cmd->args, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
		enable_termios(term);
	}
	else
		perror("fork");
}

/**
 * Execute cmds in the right order and manage the pipes between them
 *
 * @param char **cmds		commands to execute,
 * @param char **envp		environment variables
 * @param int input			input file descriptor
 * @param int output		output file descriptor
 */
void	execute_cmds(char **cmds, char **envp, int input, int output, t_term *term)
{
	size_t	i;
	int		fd[2];
	t_cmd	*cmd;
	size_t	cmds_count;

	cmds_count = ft_strlentab((const char **)cmds);
	i = 0;
	while (i < cmds_count - 1)
	{
		pipe(fd);
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		output = fd[1];
		cmd = load_command(cmds[i], envp, input, output);
		if (cmd)
		{
			cmd->toclose = fd[0];
			ft_exec_cmd(cmd, envp, term);
			destroy_cmd(cmd);
			close(fd[1]);
		}
		if (input != STDIN_FILENO)
			close(input);
		input = fd[0];
		i++;
	}
	cmd = load_command(cmds[i], envp, input, output);
	if (!cmd)
	{
		close(input);
		close(output);
		return ;
	}

	cmd->toclose = -1;

	ft_exec_cmd(cmd, envp, term);

	destroy_cmd(cmd);
	if (input != STDIN_FILENO)
		close(input);
}
