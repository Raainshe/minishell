/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_memory_leaks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksinn <ksinn@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 13:00:00 by ksinn             #+#    #+#             */
/*   Updated: 2025/03/17 12:52:31 by ksinn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

/*
** This file contains specific tests to check for memory leaks
** Run with valgrind: valgrind --leak-check=full ./test_memory_leaks
*/

void	test_gc_malloc_and_free(void)
{
	void	*ptr;
	int		i;

	printf("=== Testing gc_malloc and free_gc ===\n");
	for (i = 0; i < 100; i++)
	{
		ptr = gc_malloc(i + 1);
		if (ptr)
			printf("Allocated %d bytes at %p\n", i + 1, ptr);
		else
			printf("Failed to allocate memory\n");
		// Only add some allocations to TOKENIZER context
		if (i % 2 == 0)
		{
			gc_add_context(TOKENIZER, ptr);
			printf("Added to TOKENIZER context\n");
		}
	}
	// Free only TOKENIZER context
	printf("Freeing TOKENIZER context\n");
	gc_free_context(TOKENIZER);
	// Allocate more memory after freeing
	for (i = 0; i < 10; i++)
	{
		ptr = gc_malloc(i + 100);
		if (ptr)
			printf("Allocated %d bytes at %p\n", i + 100, ptr);
		else
			printf("Failed to allocate memory\n");
	}
	// Free all remaining allocations
	printf("Freeing all garbage collector memory\n");
	free_gc();
}

void	test_tokenizer_memory(void)
{
	char	*inputs[] = {"ls -la | grep hello",
			"echo \"this is a test with 'nested quotes'\"",
			"cat < file.txt > output.txt", NULL};
	int		i;
	char	**split_tokens;
	t_token	*tokens;
	int		count;

	printf("\n=== Testing Tokenizer Memory Management ===\n");
	i = 0;
	while (inputs[i])
	{
		printf("Tokenizing: '%s'\n", inputs[i]);
		split_tokens = ft_split_tokens(inputs[i]);
		tokens = ft_tokenize(split_tokens);
		// Print token count
		count = 0;
		while (tokens[count].type != TOKEN_END)
			count++;
		printf("Found %d tokens\n", count);
		// Free context after each tokenization
		gc_free_context(TOKENIZER);
		printf("Freed TOKENIZER context\n");
		i++;
	}
}

void	test_nested_tokenization(void)
{
	char	**split_tokens;
	int		i;
	char	input[100];
	int		token_count;
	t_token	*tokens;

	printf("\n=== Testing Nested Tokenization (without freeing) ===\n");
	for (i = 0; i < 5; i++)
	{
		sprintf(input, "command%d arg%d | grep pattern%d", i, i, i);
		printf("Tokenizing: '%s'\n", input);
		split_tokens = ft_split_tokens(input);
		// Get token count for verification
		tokens = ft_tokenize(split_tokens);
		token_count = 0;
		while (tokens[token_count].type != TOKEN_END)
			token_count++;
		printf("Created %d tokens\n", token_count);
		// Don't free between iterations to test accumulation
		printf("Not freeing this time\n");
	}
	// Free at the end
	printf("Freeing all memory at the end\n");
	free_gc();
}

int	main(void)
{
	printf("===== MEMORY LEAK TESTS =====\n\n");
	test_gc_malloc_and_free();
	test_tokenizer_memory();
	test_nested_tokenization();
	printf("\n===== MEMORY LEAK TESTS COMPLETED =====\n");
	printf("Check valgrind output for memory leak details\n");
	return (0);
}
