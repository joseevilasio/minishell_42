#include "../../includes/minishell.h"

/**
 * @brief Opens a file with specified type and mode.
 *
 * Handles the opening of a file based on the given type and mode. For input
 * files,it checks for existence and read permissions before attempting to open
 * If the  * file cannot be accessed or opened, it logs an error and sets an
 * exit status.
 *
 * @param type The type of the file, such as INFILE.
 * @param pathname A string representing the path to the file.
 * @param mode The mode in which to open the file.
 *
 * @return The file descriptor on success, or -1 on error.
 */
int	ft_open(t_redir *node, char *pathname, int mode, t_shell *sh)
{
	int	fd;
	// (void)node;

	fd = -1;
	if (node->type == INFILE)
	{
		if (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1)
		{
			if (sh->curr_fd == 0)
				ft_stderror(TRUE, "infile: %s: ", pathname);
			sh->curr_fd = -1;
			ft_exit_status(1, TRUE, FALSE);
			return (open("/dev/null", O_RDONLY));
		}
	}
	if (sh->curr_fd == 0)
		fd = open(pathname, mode, 0644);
	if (fd == -1)
	{
		if (sh->curr_fd == 0)
			ft_stderror(TRUE, "outfile: %s: ", pathname);
		sh->curr_fd = -1;
		ft_exit_status(1, TRUE, FALSE);
		//return (open("/dev/null", O_RDONLY));
		return (-1);
	}
	return (fd);
}

/**
 * @brief Searches for an execution node in branch.
 *
 * Iterates through a linked list of redirection nodes to check if any node
 * is of type EXEC. If an EXEC node is found, the function returns TRUE,
 * otherwise it returns FALSE.
 *
 * @param node A pointer to the first node in the redirection list.
 *
 * @return TRUE if an EXEC node is found, or FALSE otherwise.
 */
int	ft_search_exec(t_redir *node)
{
	int		exec;
	t_redir	*curr;

	exec = FALSE;
	curr = node->next;
	while (curr)
	{
		if (curr->type == EXEC)
		{
			exec = TRUE;
			break ;
		}
		curr = curr->next;
	}
	return (exec);
}

/**
 * @brief Handles here-document redirections.
 *
 * Manages here-document redirections by opening the temporary file,
 * duplicating its file descriptor to standard input, and then cleaning up
 * by removing the temporary file and updating the heredoc list.
 *
 * @param sh The shell structure containing the heredoc list.
 * @param node A pointer to the redirection node.
 */
void	ft_redir_heredoc(t_shell *sh, t_redir *node)
{
	char	*pathname;
	int		fd;
	t_list	*tmp;

	pathname = (char *)(*sh->heredoc_list)->content;
	tmp = (*sh->heredoc_list)->next;
	if (ft_search_exec(node) == TRUE)
	{
		fd = open(pathname, O_RDONLY);
		dup2(fd, STDIN_FILENO);
		close(fd);
		unlink(pathname);
	}
	ft_lstdelone((*sh->heredoc_list), free);
	*sh->heredoc_list = tmp;
}

// static int	ft_check_fds(t_redir *node, t_shell * sh)
// {
// 	t_redir	*curr_node;
// 	char	*pathname;

// 	curr_node = node;
// 	while (ft_is_node_type((t_node *) node, REDIR) && (curr_node->type != HEREDOC))
// 	{
// 		ft_process_token_list(curr_node->target, ft_merge_env(sh));
// 		pathname = ((t_token *)(*curr_node->target)->content)->value;
// 		if (curr_node->type == INFILE
// 			&& (access(pathname, F_OK) == -1 || access(pathname, R_OK) == -1))
// 		{
// 			ft_stderror(TRUE, "infile: %s: ", pathname);
// 			ft_exit_status(1, TRUE, FALSE);
// 			sh->curr_fd = -1;
// 			return (FALSE);
// 		}
// 		if ((curr_node->type == OUTFILE || curr_node->type == APPEND)
// 			&& (access(pathname, F_OK) == 0 && access(pathname, R_OK) == -1))
// 		{
// 			ft_stderror(TRUE, "outfile: %s: ", pathname);
// 			ft_exit_status(1, TRUE, FALSE);
// 			sh->curr_fd = -1;
// 			return (FALSE);
// 		}
// 		curr_node = node->next;
// 	}
// 	return (TRUE);
// }

/**
 * @brief Handles file redirections for the shell.
 *
 * Processes different types of file redirections, including OUTFILE, INFILE,
 * APPEND, and HEREDOC. Opens the specified file and duplicates its file
 * descriptor to the appropriate standard stream. Handles here-document
 * redirections separately.
 *
 * @param node A pointer to the redirection node.
 * @param sh The shell structure containing environment variables and settings.
 *
 * @return TRUE on success, or FALSE on failure.
 */
int	ft_redir(t_redir *node, void *next_node, t_shell *sh)
{
	int		fd;
	t_token	*tnode;
	(void)next_node;

	if (node->type == OUTFILE || node->type == INFILE || node->type == APPEND)
	{
		// if (sh->curr_fd == -1 || !ft_check_fds(node, sh))
		// {
		// 	sh->curr_fd = -1;
		// 	if (!next_node)
		// 		return (FALSE);
		// 	return (TRUE);
		// }
		// if (sh->curr_fd == 0)
		// 	ft_check_fds(node, sh);

		// ANTIGO
		// ft_process_token_list(node->target, ft_merge_env(sh));
		// tnode = (t_token *)(*node->target)->content;
		// fd = ft_open(node, tnode->value, node->mode, sh);

		//NOVO mariaoli esteve aqui
		ft_process_token_list(node->target, ft_merge_env(sh));
		if (!*node->target) //checking twice so I won't access invalid memory after processing token
		{
			ft_stderror(FALSE, "ambiguous redirect");
			ft_exit_status(1, TRUE, FALSE);
			return (FALSE);
		}
		else
		{
			tnode = (t_token *)(*node->target)->content;
			fd = ft_open(node, tnode->value, node->mode, sh);
		}

		//if (sh->curr_fd == 0)
		// if (sh->curr_fd == -1)
		// {
		// 	//fd = open("/dev/null", O_RDONLY);
		// if (sh->curr_fd == -1)
		// {
		// 	// if (!next_node)
		// 	// 	return (FALSE);
		// 	return (TRUE);
		// }		
		// 	close(fd);
		// 	return (TRUE);
		// }
		if (node->type == OUTFILE || node->type == APPEND)
		{
			dup2(fd, STDOUT_FILENO);
		}
		else
			dup2(fd, STDIN_FILENO);
		close(fd);
		return (TRUE);
	}
	else if (node->type == HEREDOC)
	{
		ft_redir_heredoc(sh, node);
		return (TRUE);
	}
	return (FALSE);
}

