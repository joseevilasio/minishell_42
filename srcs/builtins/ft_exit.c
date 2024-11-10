#include "../../includes/minishell.h"

/**
 * @brief Exits the program with a specified exit status and displays an exit message.
 *
 * This function prints "exit" to the standard error output (file descriptor 2) and then calls
 * `ft_exit_status` to set the exit status. If `exit_` is `TRUE`, it terminates the program using 
 * the current status.
 *
 * @param exit_ If `TRUE`, the function will terminate the program with the current exit status.
 *              If `FALSE`, it only updates the exit status without terminating.
 */
void	ft_exit(int exit_)
{
	ft_putendl_fd("exit", 2);
	ft_exit_status(0, TRUE, exit_);
}
