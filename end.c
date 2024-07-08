/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 21:39:08 by julthoma          #+#    #+#             */
/*   Updated: 2024/02/13 21:39:08 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_end(char **cmd, char **paths, int io[2], int toclose)
{
	ft_freetab(cmd);
	ft_freetab(paths);
	close(io[0]);
	close(io[1]);
	if (toclose != -1)
		close(toclose);
	exit(1);
}

void	ft_close(int io[2], int cmds_len, int fd[2])
{
	close(io[0]);
	close(io[1]);
	if (cmds_len < 3)
	{
		close(fd[0]);
		close(fd[1]);
	}
}
