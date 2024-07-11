/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_maker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/11 09:51:47 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Destroy a command structure
 *
 * @param t_cmd	*cmd
 */
void	destroy_cmd(t_cmd *cmd)
{
	ft_freetab(cmd->args);
	free(cmd->path);
	free(cmd);
}

/**
 * @brief Load a command structure properly filled with the given data
 *
 * @param char *cmd
 * @param char **envp
 * @param int input
 * @param int output
 * @return t_cmd *
 */
t_cmd	*load_command(char *cmd, char **envp, int input, int output)
{
	t_cmd	*res;

	res = malloc(sizeof(t_cmd));
	if (res == NULL)
		return (NULL);
	res->args = ft_split(cmd, " ");
	res->path = get_path(res->args[0], envp);
	if (res->path == NULL)
	{
		ft_freetab(res->args);
		free(res);
		return (NULL);
	}
	res->input = input;
	res->output = output;
	return (res);
}
