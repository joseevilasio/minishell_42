#include "../../includes/minishell.h"

/**
 * @brief Checks if a variable exists in the environment.
 *
 * This function iterates through the `my_envp` environment array to check if 
 * a variable with the name `str` exists. It compares `str` with each env
 * variable up to the length of `str` and ensures an exact match followed
 * by an '='.
 * If the variable is found, it returns the index of the environment variable.
 * Otherwise, it returns 0.
 *
 * @param str The variable name to search for.
 * @param my_envp A pointer to the array of environment variables.
 * 
 * @return The length of the environment variable; otherwise, 0.
 */
static size_t	is_var(char *str, char **my_envp)
{
	size_t	index_envp;
	size_t	flag;
	size_t	size_var;

	flag = 0;
	index_envp = 0;
	size_var = ft_strlen(str);
	while ((my_envp)[index_envp])
	{
		if (ft_strncmp(str, (my_envp)[index_envp], size_var) == 0
			&& (my_envp)[index_envp][size_var] == '=')
			flag++;
		index_envp++;
	}
	if (flag > 0)
		return (index_envp);
	else
		return (0);
}

/**
 * @brief Deletes a variable from the environment.
 *
 * This function removes a variable with the name `str` from the `my_envp` 
 * environment array. It compares `str` with each environment variable up to 
 * the length of `str` and ensures an exact match followed by an '='.
 * If a match is found, the variable is removed and the memory is freed.
 * A new environment array is created without the removed variable and the
 * old environment array is freed.
 *
 * @param str The name of the variable to be deleted.
 * @param my_envp A pointer to the environment variable array.
 * @param size_env The size of the environment array.
 * 
 * @return 0 on success, or -1 if memory allocation fails.
 */
static int	delete_var(char *str, char ***my_envp, size_t size_env)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	**new_envp;

	size = ft_strlen(str);
	i = 0;
	j = 0;
	new_envp = (char **) malloc(size_env * sizeof(char *));
	if (!new_envp)
		return (ft_exit_status(1, TRUE, FALSE), -1); //utilizar funcao malloc
	while (i < size_env)
	{
		if (ft_strncmp(str, (*my_envp)[i], size) == 0
			&& (*my_envp)[i][size] == '=')
		{
			free((*my_envp)[i]);
			i++;
		}
		new_envp[j++] = (*my_envp)[i++];
	}
	new_envp[j] = NULL;
	free((*my_envp));
	*my_envp = new_envp;
	return (0);
}

/**
 * @brief Unsets (removes) specified environment variables.
 *
 * This function removes variables from the `my_envp` array. It iterates
 * through `argv` and checks each variable using the `is_var` function. If
 * the variable exists, it is removed using `delete_var`. If `argc` is 1, no
 * variables are specified, and the function returns without changes.
 *
 * @param argc The count of arguments. If `argc > 1`, variables are specified.
 * @param argv The list of arguments; each is a variable name to unset.
 * @param sh The shell structure containing `global` and `local` environments.
 *
 * @return Exit status: 0 indicates successful execution.
 */
int	ft_unset(int argc, char **argv, t_shell *sh)
{
	size_t	size_env;

	if (argc == 1)
		return (ft_exit_status(0, TRUE, FALSE));
	++argv;
	while (*argv)
	{
		if ((*sh->global))
		{
			size_env = is_var(*argv, sh->global);
			if (size_env > 0)
				delete_var(*argv, &(sh->global), size_env);
		}
		if ((*sh->local))
		{
			size_env = is_var(*argv, sh->local);
			if (size_env > 0)
				delete_var(*argv, &(sh->local), size_env);
		}
		argv++;
	}
	return (ft_exit_status(0, TRUE, FALSE));
}

//missing delete VAR without '='