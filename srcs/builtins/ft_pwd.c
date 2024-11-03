#include "../../includes/minishell.h"

int	ft_pwd(void)
{
	char	current_path[1024];

	if (getcwd(current_path, 1024) != NULL)
		printf("%s\n", current_path);
	else
		return (ft_exit_status(1, TRUE, TRUE));
	return (ft_exit_status(0, TRUE, TRUE));
}
