#ifndef BUILTINS_H
#define BUILTINS_H

//ft_env.c
void	ft_env(char **my_envp);

//ft_cd.c
void	ft_cd(int argc, const char *new_dir, char **my_envp);
char	*ft_getenv(char *env, char **my_envp);

//ft_echo.c
void	ft_echo(char **args);

#endif