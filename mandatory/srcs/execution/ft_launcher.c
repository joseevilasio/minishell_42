#include "../../includes/minishell.h"

/**
 * @brief Forks a child process to execute a command with redirection.
 *
 * Creates a child process using fork, sets up file descriptor redirections,
 * and executes the command. Handles errors and manages the shell state.
 *
 * @param fds An array of file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 * @param node The current command node to be executed.
 * @param next_node The next command node in the pipeline.
 *
 * @return The process ID of the child process.
 */
pid_t	ft_child_process(int *fds, t_shell *sh, void *node, void *next_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_stderror(TRUE, "");
		ft_exit_status(1, TRUE, FALSE);
	}
	if (pid == 0)
	{
		close_original_fds(sh);
		close(fds[0]);
		if (next_node != NULL && (sh->prev && ((t_redir *)sh->prev)->type
				!= OUTFILE) && ((t_redir *)sh->prev)->type != APPEND)
		{
			dup2(fds[1], STDOUT_FILENO);
		}
		close(fds[1]);
		ft_exec(((t_exec *)node)->args, sh);
	}
	return (pid);
}

/**
 * @brief Manages the parent process in a pipeline or command execution.
 *
 * Handles signals, waits for the child process to finish, and updates the
 * exit status. Manages file descriptors for pipeline continuation or restores
 * the original file descriptors if the pipeline ends.
 *
 * @param curr_fds The current file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 * @param node The current node in the command pipeline.
 * @param pid The process ID of the child process.
 */
void	ft_parent_process(int *curr_fds, t_shell *sh, void *node, pid_t pid)
{
	int	status;

	sh->error_fd = 0;
	if (!node)
	{
		ft_signal(CHILD_);
		close_fds(curr_fds);
		if (pid != -1 && waitpid(pid, &status, 0) != -1)
		{
			if (WIFEXITED(status))
				ft_exit_status(WEXITSTATUS(status), TRUE, FALSE);
			else if (WIFSIGNALED(status))
				ft_exit_status(WTERMSIG(status) + 128, TRUE, FALSE);
		}
		return (ft_restore_original_fds(sh));
	}
	if (node)
		dup2(curr_fds[0], STDIN_FILENO);
	close_fds(curr_fds);
	if (sh->prev && (((t_redir *)sh->prev)->type == OUTFILE
			|| ((t_redir *)sh->prev)->type == APPEND))
		dup2(sh->stdout_, STDOUT_FILENO);
	ft_launcher(node, ((t_node *)node)->right, NULL, sh);
}

/**
 * @brief Executes a command in a child process and manages file descriptors.
 *
 * This function creates a pipe, executes the command in a child process, and 
 * manages the file descriptors. If `sh->error_fd` is 0, it creates the pipe 
 * and calls `ft_child_process` to execute the command in the child process.
 * Then, it calls `ft_parent_process` to manage the parent process.
 *
 * @param node The current node of the command list.
 * @param next_node The next node of the command list.
 * @param fds An array of file descriptors for the pipe.
 * @param sh A pointer to the t_shell structure containing the shell state.
 */
void	ft_launcher_exec(void *node, void *next_node, int *fds, t_shell *sh)
{
	pid_t	pid;

	pid = -1;
	if (sh->error_fd == 0)
	{
		if (pipe(fds) == -1)
			return (ft_exit_status(1, TRUE, FALSE), ft_stderror(TRUE, ""));
		pid = ft_child_process(fds, sh, node, next_node);
	}
	ft_parent_process(fds, sh, next_node, pid);
}

/**
 * @brief Launches commands in a pipeline or executes redirections.
 *
 * Recursively processes a command node and its next node, handling
 * pipes and redirections. Forks child processes to execute commands
 * and manages parent processes.
 *
 * @param node The current command node to be processed.
 * @param next_node The next command node in the pipeline.
 * @param curr_fds The current file descriptors for the pipe.
 * @param sh The shell structure containing the execution state and environment
 */
void	ft_launcher(void *node, void *next_node, int *curr_fds, t_shell *sh)
{
	ft_save_original_fds(sh);
	if (!node)
		return ;
	else if (((t_node *)node)->type == PIPE)
	{
		sh->prev = node;
		ft_launcher(((t_node *)node)->left, ((t_node *)node)->right, sh->fds, \
			sh);
	}
	else if (ft_redir(((t_redir *)node), sh))
	{
		sh->prev = node;
		if (!((t_redir *)node)->next)
		{
			if (next_node)
				if (pipe(curr_fds) == -1)
					return (ft_exit_status(1, TRUE, FALSE), \
						ft_stderror(TRUE, ""));
			ft_parent_process(curr_fds, sh, next_node, -1);
		}
		ft_launcher(((t_redir *)node)->next, next_node, curr_fds, sh);
	}
	else if (((t_exec *)node)->type == EXEC)
		ft_launcher_exec(node, next_node, curr_fds, sh);
}

/**
 * @brief Manages the execution of commands in the syntax tree.
 *
 * This function orchestrates the execution of commands by handling heredoc
 * processing, checking if the command is a single built-in, and launching
 * the appropriate execution flow. It also restores the command-line interface
 * state after execution.
 *
 * @param tree The syntax tree representing the parsed commands.
 * @param sh The shell structure containing execution context and state.
 */
void	ft_launcher_manager(void *tree, t_shell *sh)
{
	if (tree)
	{
		sh->root = tree;
		ft_search_heredoc(tree, sh);
		if (sh->run == TRUE
			&& !ft_single_command(tree, sh))
		{
			ft_signal(DEFAULT_);
			ft_launcher(tree, ((t_node *)tree)->right, NULL, sh);
		}
	}
	ft_restore_cli(sh, tree);
}