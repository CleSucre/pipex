/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 18:27:30 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 12:22:21 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/include/libft.h"
# include "custom_termios.h"
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_cmd
{
	char	*cmd;
	char	**args;
	char	*path;
	int		input;
	int		output;
	int		toclose;
}			t_cmd;

// ########################################################
// #					COMMAND_MAKER					  #
// ########################################################

void	destroy_cmd(t_cmd *cmd);
t_cmd	*load_command(char *cmd, char **envp, int input, int output);

// ########################################################
// #					       PATH		    			  #
// ########################################################

char	*get_path(char *cmd, char **envp);

// ########################################################
// #					    EXECUTOR	    			  #
// ########################################################

void	execute_cmds(char **cmds, char **envp, int input, int output, t_term *term);

#endif
