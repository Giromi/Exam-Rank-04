#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void ft_putstr(char *str)	// 에러를 출력해주는 함수
{
	int i = 0;
	while(str[i])			// while 돌면서 문자 출력
		i++;
	write(2, str, i);		//
}

int ft_execute(char **argv, int i, int tmp_fd, char **envp)
{
	argv[i] = NULL;
	close(tmp_fd);
	execve(argv[0], argv, envp);
	ft_putstr("error: cannot execute ");
	ft_putstr(argv[0]);
	ft_putstr("\n");
	return 1;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	int i = 0;
	int pid = 0;
	int tmp_fd = dup(0);
	int fd[2];

	while(argv[i] && argv[i + 1])
	{
		argv = &argv[i + 1];		//오... 주소를 바꿔도 괜찮나보구먼..
		i = 0;
		while(argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if(strcmp(argv[0], "cd") == 0)
		{
			if(i != 2)
				ft_putstr("error: cd: bad arguments\n");
			else if(chdir(argv[1]) != 0)
			{
				ft_putstr("error: cd: cannot change directory to ");
				ft_putstr(argv[1]);
				ft_putstr("\n");
			}
		}
		else if(argv != &argv[i] && (argv[i] == NULL || strcmp(argv[i], ";") == 0))
		{
			pid = fork();
			if(pid == 0)
			{
				dup2(tmp_fd, 0);
				if(ft_execute(argv, i, tmp_fd, envp))
					return 1;
			}
			else
			{
				close(tmp_fd);
				waitpid(-1, 0, 0);
				tmp_fd = dup(0);
			}
		}
		else if(argv != &argv[i] && strcmp(argv[i], "|") == 0)
		{
			pipe(fd);
			pid = fork();
			if(pid == 0)
			{
				dup2(tmp_fd, 0);
				dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
				if(ft_execute(argv, i, tmp_fd, envp))
					return 1;
			}
			else
			{
				close(tmp_fd);
				close(fd[1]);
				waitpid(-1, 0, 0);
				tmp_fd = dup(fd[0]);
				close(fd[0]);
			}
		}
	}
	close(tmp_fd);
	return 0;
}
