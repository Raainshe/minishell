/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_tokenizer_gc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:35:00 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 12:38:50 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>

void	print_token_info(t_token *tokens)
{
	int		i;
	char	*type_names[] = {"WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
			"HERE_DOC", "APPEND", "END"};

	i = 0;
	while (tokens[i].type != TOKEN_END)
	{
		printf("Token %d: Content='%s', Type=%s\n", i, tokens[i].content,
			type_names[tokens[i].type]);
		i++;
	}
	printf("Token %d: END\n", i);
}

void	test_simple_command(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "ls -la";
	printf("\n=== Test Simple Command ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_pipes(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "ls -la | grep a | wc -l";
	printf("\n=== Test Pipes ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_redirections(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "cat < input.txt > output.txt";
	printf("\n=== Test Redirections ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_here_doc_and_append(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "cat << EOF >> output.txt";
	printf("\n=== Test Here Doc and Append ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_quotes(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "echo \"Hello 'World'\" 'Single \"Quotes\"'";
	printf("\n=== Test Quotes ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_complex_command(void)
{
	char	*input;
	char	**split_tokens;
	t_token	*tokens;

	input = "cat file.txt | grep \"pattern with 'quotes'\" | sort > output.txt";
	printf("\n=== Test Complex Command ===\n");
	printf("Input: '%s'\n", input);
	split_tokens = ft_split_tokens(input);
	tokens = ft_tokenize(split_tokens);
	print_token_info(tokens);
	gc_free_context(TOKENIZER);
	printf("Memory freed for TOKENIZER context\n");
}

void	test_edge_cases(void)
{
	int		i;
	char	**split_tokens;
	t_token	*tokens;

	char *inputs[] = {"",
						"   ",
						">",
						"ls -la ; echo test",
						// Semi-colon should be its own token
						"ls|grep a", // No spaces around pipe
						"ls > >",    // Multiple redirects
						NULL};
	i = 0;
	printf("\n=== Test Edge Cases ===\n");
	while (inputs[i])
	{
		printf("\nInput: '%s'\n", inputs[i]);
		split_tokens = ft_split_tokens(inputs[i]);
		tokens = ft_tokenize(split_tokens);
		print_token_info(tokens);
		gc_free_context(TOKENIZER);
		printf("Memory freed for TOKENIZER context\n");
		i++;
	}
}

void	test_garbage_collector(void)
{
	void *ptr1, *ptr2, *ptr3;
	printf("\n=== Test Garbage Collector ===\n");
	// Allocate memory and add to tokenizer context
	ptr1 = gc_malloc(100);
	gc_add_context(TOKENIZER, ptr1);
	printf("Allocated ptr1 (100 bytes) and added to TOKENIZER context\n");
	ptr2 = gc_malloc(200);
	gc_add_context(TOKENIZER, ptr2);
	printf("Allocated ptr2 (200 bytes) and added to TOKENIZER context\n");
	// Allocate but don't add to any context - should be leaked
	ptr3 = gc_malloc(300);
	printf("Allocated ptr3 (300 bytes) but not added to any context\n");
	// Free tokenizer context
	gc_free_context(TOKENIZER);
	printf("Freed TOKENIZER context - ptr1 and ptr2 should be freed\n");
	// Free all memory
	free_gc();
	printf("Called free_gc() - all managed memory should be freed\n");
	// Free ptr3 manually to avoid leaks
	free(ptr3);
	printf("Manually freed ptr3\n");
}

int	main(void)
{
	printf("===== TOKENIZER AND GARBAGE COLLECTOR TESTS =====\n");
	test_simple_command();
	test_pipes();
	test_redirections();
	test_here_doc_and_append();
	test_quotes();
	test_complex_command();
	test_edge_cases();
	test_garbage_collector();
	printf("\n===== ALL TESTS COMPLETED =====\n");
	return (0);
}
