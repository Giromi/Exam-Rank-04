
#ifndef UTILS_H
# define UTILS_H

enum e_std
{
	STDIN, STDOUT, STDERR
};

void	ft_putstr_fd(char *str, int fd);
int		ft_strlen(char *str);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *str, int fd);

#endif
