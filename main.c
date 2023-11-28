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

static void ft_free_tab(char **tab)
{
    int i;

    i = 0;
    while (tab[i])
        free(tab[i++]);
    free(tab);
}

static char *ft_getcmd_path(char *name, char **envp)
{
    char *path;
    char **paths;
    int i;

    i = 0;
    while (ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    path = ft_strdup(envp[i] + 5);
    paths = ft_split(path, ':');
    i = 0;
    while (paths[i])
    {
        path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(path, name);
        if (access(path, X_OK) == 0)
        {
            ft_free_tab(paths);
            return (path);
        }
        free(path);
        i++;
    }
    ft_free_tab(paths);
    return (NULL);
}

static void exec_command(char **cmd, char **envp)
{
    char *cmd_path;

    cmd_path = ft_getcmd_path(cmd[0], envp);
    if (cmd_path == NULL)
    {
        ft_printf("command not found\n");
        exit(1);
    }
    execve(cmd_path, cmd, envp);
    perror("execve");
    exit(1);
}

static void pipex(int fd[2], char **cmd1, char **cmd2, char **envp)
{
    int pid;

    (void)envp;
    pid = fork();
    if (pid == 0)
    {
        dup2(fd[0], STDOUT_FILENO);
        close(fd[1]);
        //execvp(cmd1[0], cmd1);
        exec_command(cmd1, envp);
    }
    else if (pid == 1)
    {
        dup2(fd[1], STDIN_FILENO);
        close(fd[0]);
        //execvp(cmd2[0], cmd2);
        exec_command(cmd2, envp);
    }
    else if (pid == -1)
    {
        perror("fork");
    }

    ft_free_tab(cmd1);
    ft_free_tab(cmd2);
}

int main(int argc, char **argv, char **envp)
{
    int pipefd[2];
    int pid;

    (void)argc;
    //if (argc != 5)
    //    return (1);
    pipe(pipefd);
    pid = fork();
    if (pid == 0)
    {
        pipex(pipefd, ft_split(argv[1], ' '), ft_split(argv[2], ' '), envp);
    }
    else if (pid == 1)
    {
        ft_printf("not dev\n");
    }
    else if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    return (0);
}
