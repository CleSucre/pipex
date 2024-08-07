/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 18:27:30 by julthoma          #+#    #+#             */
/*   Updated: 2024/01/10 18:27:30 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/include/libft.h"
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>

void	ft_end(char **cmd, char **paths, int io[2], int toclose);
void	ft_close(int io[2], int cmds_len, int fd[2]);

#endif
