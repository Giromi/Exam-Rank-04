/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minsuki2 <minsuki2@student.42seoul.kr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:34:24 by minsuki2          #+#    #+#             */
/*   Updated: 2022/11/01 17:29:04 by minsuki2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

enum e_return
{
	ERROR = -1, SUCCESS
};

enum e_fd
{
	READ, WRITE
};

static int _count_metachar(char *argv[])
{
	int j = -1;

	while (argv[++j] && strcmp(argv[j], ";") && strcmp(argv[j], "|"));
	return (j);
}

static void _builtin_cd(char *argv[], int cnt)
{
	if (cnt != 2)
		ft_putstr_fd("error: cd: bad arguments\n", STDERR);
	else if (chdir(argv[1]) == ERROR)
	{
		ft_putstr_fd("error: cd: cannot change directory to ", STDERR);
		ft_putstr_fd(argv[1], STDERR);
		ft_putchar_fd('\n', STDERR);
	}
}

int my_execve(char **argv, char *envp[], int i, int backup_fd)
{

	argv[i] = NULL;
	close(backup_fd);
	execve(argv[0], argv, envp);
	ft_putstr_fd("error: cannot execute ", STDERR);
	ft_putstr_fd(argv[0], STDERR);
	ft_putchar_fd('\n', STDERR);
	exit(1);
}

static void _execute_cmd(char *argv[], char *envp[], int i, int backup_fd)
{
	pid_t	pid;

	pid = fork();
	if (!pid)
	{
		dup2(backup_fd, 0);
		my_execve(argv, envp, i, backup_fd);
	}
	else
	{
		close(backup_fd);
		waitpid(-1, 0, 0);
		backup_fd = dup(STDIN);
	}
}

static void _pipe_connect(char *argv[], char *envp[], int i, int backup_fd, int fd[2])
{
	pid_t	pid;

	pipe(fd);
	pid = fork();
	else if (!pid)
	{
		dup2(backup_fd, STDIN);
		dup2(fd[WRITE], STDOUT);
		close(fd[WRITE]);
		close(fd[READ]);
		my_execve(argv, envp, i, backup_fd);
	}
	else
	{
		close(backup_fd);
		close(fd[WRITE]);
		waitpid(-1, 0, 0);
		backup_fd = dup(fd[READ]);
		close(fd[READ]);
	}
}

int main(int argc, char *argv[], char *envp[])
{
	int		i = 0;
	int		backup_fd = dup(STDIN);
	int		fd[2];

	if (argc <= 1)
		return (SUCCESS);
	while (argv[i++] && argv[i])
	{
		argv = &argv[i];
		i = _count_metachar(argv);
		if (argv == &argv[i])
			continue ;
		else if (!strcmp(argv[0], "cd"))
			_builtin_cd(argv, i);
		else if (!argv[i] || !strcmp(argv[i], ";"))
			_execute_cmd(argv, envp, i, backup_fd);
		else if (!argv[i] || !strcmp(argv[i], "|"))
			_pipe_connect(argv, envp, i, backup_fd, fd);
	}
	close(backup_fd);
	return (SUCCESS);
}
