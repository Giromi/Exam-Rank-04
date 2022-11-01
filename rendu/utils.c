
#include "utils.h"
#include <unistd.h>

void ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

int ft_strlen(char *str)
{
	int i = -1;

	if (!str)
		return (-1);
	while (str[++i]);
	return (i);
}

void ft_putstr_fd(char *str, int fd)
{
	write(fd, str, ft_strlen(str));
}

void ft_putendl_fd(char *str, int fd)
{
	ft_putstr_fd(str, fd);
	ft_putchar_fd('\n', fd);
}

void ft_puterror(char *msg, char *arg)
{
	ft_putstr_fd("error: cannot execute ", STDERR);
	ft_putendl_fd(arg, STDERR);
}
