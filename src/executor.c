/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <julthoma@student.42angouleme.f>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 09:51:47 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 19:49:55 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

// Gestionnaire de signal pour SIGCHLD
void sigchld_handler(int signum)
{
	int status;
	(void)signum;

	// Attendre tous les enfants terminés
	while (waitpid(-1, &status, WNOHANG) > 0)
	{
		// Vous pouvez ajouter du code ici pour traiter le statut si nécessaire
	}
}

/**
 * @brief Execute the command
 *
 * @param cmd Structure contenant la commande à exécuter
 * @param envp Environnement
 * @param term Structure de terminal
 * @param next_cmds Commandes suivantes
 */
static void ft_exec_cmd(t_cmd *cmd, char **envp, t_term *term, int input, int output, int toclose, char **next_cmds)
{
	int pipefd[2];
	char buf;

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	pid_t pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]); // Ferme l'extrémité de lecture du pipe dans le processus enfant

		if (input != STDIN_FILENO)
		{
			dup2(input, STDIN_FILENO);
			close(input);
		}
		if (output != STDOUT_FILENO)
		{
			dup2(output, STDOUT_FILENO);
			close(output);
		}
		if (toclose != -1)
			close(toclose);

		// Notify the parent that the child is ready
		write(pipefd[1], "1", 1);
		close(pipefd[1]); // Close the write end of the pipe

		if (execve(cmd->path, cmd->args, envp) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid > 0)
	{
		close(pipefd[1]); // Ferme l'extrémité d'écriture du pipe dans le processus parent

		// Attend que l'enfant signale qu'il est prêt
		read(pipefd[0], &buf, 1);
		close(pipefd[0]); // Close the read end of the pipe

		// Maintenant que l'enfant a signalé qu'il est prêt, lance les autres commandes
		if (next_cmds)
			execute_cmds(next_cmds, envp, input, output, term);

		ft_printf("child process %d started\n", pid);
	}
	else
		perror("fork");
}

void execute_cmds(char **cmds, char **envp, int input, int output, t_term *term)
{
	int fd[2];
	t_cmd *cmd;
	size_t cmds_count;

	cmds_count = ft_strlentab((const char **)cmds);
	if (cmds_count > 0)
	{
		if (cmds_count > 1)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			cmd = load_command(cmds[0], envp, input, fd[1]);
			if (cmd)
			{
				cmd->toclose = fd[0];
				ft_exec_cmd(cmd, envp, term, input, fd[1], fd[0], cmds + 1);  // Appel récursif pour exécuter la commande suivante
				destroy_cmd(cmd);
				close(fd[1]);
			}
			if (input != STDIN_FILENO)
				close(input);
		}
		else
		{
			cmd = load_command(cmds[0], envp, input, output);
			if (!cmd)
			{
				if (input != STDIN_FILENO)
					close(input);
				if (output != STDOUT_FILENO)
					close(output);
				return;
			}

			cmd->toclose = -1;
			ft_exec_cmd(cmd, envp, term, input, output, -1, NULL);  // Dernière commande, pas de commandes suivantes
			destroy_cmd(cmd);
			if (input != STDIN_FILENO)
				close(input);
		}
	}
}
