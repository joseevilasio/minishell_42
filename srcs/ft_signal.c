#include "../includes/minishell.h"

/**
 * @brief Signal handler for the parent process, handling interrupts (SIGINT).
 *
 * This function handles the SIGINT signal in the parent process, typically
 * triggered by pressing `Ctrl + C`. It performs the following actions:
 * - Writes a newline character to standard output.
 * - Sets the exit status to 130, indicating that the process was terminated 
 *   by SIGINT.
 * - Resets the current input line and refreshes the `readline` prompt.
 *
 * @param sig The signal number received, expected to be SIGINT (signal 2).
 */
static void	sig_parent_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	ft_exit_status(130, TRUE, FALSE);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Signal handler for child processes, managing SIGINT and SIGQUIT
 *       signals.
 *
 * This function handles specific signals for child processes:
 * - If `SIGINT` (signal 2) is received, it writes a newline character to 
 *   standard output.
 * - If `SIGQUIT` (signal 3) is received, it outputs "Quit (core dumped)"
 *   to standard output.
 * 
 * This provides feedback when a child process is interrupted (`Ctrl + C`) or 
 * quit (`Ctrl + \`), ensuring proper message output without affecting the
 * parent process.
 *
 * @param sig The signal number received, which can be SIGINT or SIGQUIT.
 */
static void	sig_child_handler(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	if (sig == SIGQUIT)
		ft_putendl_fd("Quit (core dumped)", 1);
}

/**
 * @brief Signal handler for heredoc interruptions.
 *
 * Handles the `SIGINT` signal during heredoc input by:
 * - Writing a newline to the standard output.
 * - Closing the standard input file descriptor.
 * - Setting the exit status to 130.
 *
 * @param sig The signal number (expected to be `SIGINT`).
 */
static void	sig_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		close(STDIN_FILENO);
		ft_exit_status(130, TRUE, FALSE);
	}
}

/**
 * @brief Configures signal handling based on process type (PARENT, HEREDOC,
 *        DEFAULT, CHILD).
 *
 * This function sets up signal handlers depending on the type of process:
 * - For `PARENT`, it ignores `SIGTSTP` and `SIGQUIT`, and handles `SIGINT` 
 *   with `sig_parent_handler`.
 * - For `HEREDOC`, it ignores `SIGQUIT` and leaves space to set up additional
 *   handlers as needed.
 * - For `DEFAULT`, it restores default behavior for `SIGQUIT` and `SIGINT`.
 * - For `CHILD`, it handles `SIGINT` and `SIGQUIT` with `sig_child_handler`.
 * 
 * Using different handlers allows precise control over parent, child, and 
 * heredoc process behaviors, ensuring proper signal response for interactive
 * shells and subprocesses.
 *
 * @param type Specifies the type of process (PARENT, HEREDOC, DEFAULT, CHILD).
 */
void	ft_signal(int type)
{
	signal(SIGTSTP, SIG_IGN);
	if (type == PARENT_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_parent_handler);
	}
	if (type == HEREDOC_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, sig_heredoc_handler);
	}
	if (type == DEFAULT_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	if (type == CHILD_)
	{
		signal(SIGTSTP, SIG_IGN);
		signal(SIGINT, sig_child_handler);
		signal(SIGQUIT, sig_child_handler);
	}
}
