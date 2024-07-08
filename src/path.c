/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:00:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/08 11:00:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @brief Check if the path and free the paths if correct
 *
 * @param char *path
 * @param char **paths
 * @return int 1 if the path is correct, 0 otherwise
 */
static int	check_path(char *path, char **paths)
{
	if (access(path, X_OK) == 0)
	{
		ft_freetab(paths);
		return (1);
	}
	free(path);
	return (0);
}

/**
 * @brief Get the path of the command to execute
 *
 * @param char *cmd
 * @param char **envp
 * @param int io[2]
 * @param int toclose
 * @return
 */
char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	*path;
	char	**paths;
	char	*tmp;

	if (access(cmd, X_OK) == 0)
		return (cmd);
	i = 0;
	while (ft_strncmp(envp[i], "PATH=", 5))
		i++;
	paths = ft_split(envp[i] + 5, ":");
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (check_path(path, paths))
			return (path);
	}
	ft_freetab(paths);
	return (NULL);
}
