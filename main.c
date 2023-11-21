#include "pipex.h"

static void pipex(char **argv)
{
    int fd[2];
    int pid;

    pipe(fd);
    pid = fork();
    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execve(argv[2], &argv[2], NULL);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execve(argv[3], &argv[3], NULL);
    }
}

int main(int argc, char **argv, char **envp)
{
    (void)envp;
    if (argc != 5)
        return (1);
    pipex(argv);
    return (0);
}
