#include "../../includes/minishell.h"

/**
 * @brief Handles metacharacter tokens, adding them to the token list.
 * 
 * This function appends the metacharacter at the current position to `value`. If it
 * detects a double metacharacter (like `>>` or `<<`), it appends the second character
 * as well and increments the index. The constructed metacharacter token is then added 
 * to the token list.
 * 
 * @param value Pointer to the string being built for the current token.
 * @param s Pointer to the input string at the current parsing position.
 * @param i The current index in the input string.
 * @param token_list Pointer to the list of tokens.
 * @return Updated index after processing the metacharacter.
 */
static int	ft_handle_metachar(char **value, char *s, int i, t_list **token_list)
{
	*value = ft_charjoin(*value, s[0]);
	if ((s[0] == '>' || s[0] == '<') && ft_strchr(METACHARS, s[1]))
	{
		*value = ft_charjoin(*value, s[1]);
		i++;
	}
	ft_add_to_token_list(value, token_list);
	return (i);
}

/**
 * @brief Handles quoted sections in the input, adding them to the token string.
 * 
 * This function appends characters within a quoted section to `value`, including
 * the opening and closing quotes. It returns the updated index after the closing
 * quote.
 * 
 * @param value Pointer to the string being built for the current token.
 * @param s Pointer to the input string.
 * @param i The current index in the input string.
 * @param quote The quote character (`'` or `"`).
 * @return Updated index after processing the quoted section.
 */
static int	ft_handle_quotes(char **value, char *s, int i, char quote)
{
	*value = ft_charjoin(*value, s[i]);
	i++;
	while (s[i] != quote)
	{
		*value = ft_charjoin(*value, s[i]);
		i++;
	}
	*value = ft_charjoin(*value, s[i]);
	return (i);
}

/**
 * @brief Processes the input string and creates tokens based on metacharacters and unquoted spaces.
 * 
 * This function iterates through the input string, identifying tokens separated by
 * unquoted spaces or metacharacters. It adds each constructed token to the token
 * list, handling quoted and metacharacter sequences appropriately.
 * 
 * @param s The input string to tokenize.
 * @param token_list Pointer to the list of tokens.
 */
static void	ft_process_tokens(char *s, t_list **token_list)
{
	char	*value;
	int		i;

	i = 0;
	value = NULL;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
		{
			if (ft_strchr(METACHARS, s[i]))
			{
				if (value)
					ft_add_to_token_list(&value, token_list);
				i = ft_handle_metachar(&value, &s[i], i, token_list);
			}
			else if (s[i] == SQUOTE || s[i] == DQUOTE)
				i = ft_handle_quotes(&value, s, i, s[i]);
			else
				value = ft_charjoin(value, s[i]);
		}
		if (ft_isspace(s[i]) && value)
			ft_add_to_token_list(&value, token_list);
		i++;
	}
	ft_add_to_token_list(&value, token_list);
}

/**
 * @brief Validates and updates export tokens in a list based on position and context.
 * 
 * Scans the token list, identifying tokens marked as `EXPORT` or `EXPORT_AP` that
 * are not at the beginning or are following a non-pipe command. These tokens have their type
 * changed to `EXEC` to ensure proper command handling during execution.
 * 
 * @param list Double pointer to the head of the list of tokens.
 */
void	ft_validate_export_tokens(t_list **list)
{
	t_list	*current;
	t_token	*token;
	t_list	*prev;
	int	pos;

	pos = 0;
	current = *list;
	prev = NULL;
	while (current)
	{
		token = (t_token *)current->content;
		if ((token->type == EXPORT || token->type == EXPORT_AP) && pos > 0
				&& !(((t_token *)prev->content)->type == PIPE))
			token->type = EXEC;
		pos++;
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Creates and processes a token list from a given string.
 * 
 * Allocates memory for a new token list, tokenizes the input string, and applies
 * necessary processing steps such as handling `EXPORT` tokens. Returns a pointer to
 * the created list.
 * 
 * @param s Input string to be tokenized and processed.
 * @return A double pointer to the head of the newly created token list.
 */

/**
 * @brief Creates and processes token list from the input string.
 * 
 * This function allocates memory for a token list, processes the input string to 
 * identify tokens, populates the list and applies necessary processing steps such as
 * handling `EXPORT` tokens. It frees the input string after processing.
 * 
 * @param s The input string to tokenize.
 * @return A pointer to the list of tokens, or NULL if allocation fails.
 */
t_list	**ft_create_token_list(char *s)
{
	t_list	**token_list;

	token_list = (t_list **)malloc(sizeof(t_list **));
	if (!token_list)
		return (NULL); // ft_error_handler();
	*token_list = NULL;
	ft_process_tokens(s, token_list);
	ft_validate_export_tokens(token_list);
	return (free(s), token_list);
}
