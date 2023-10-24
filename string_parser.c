/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan, Monil
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/
	if (buf == NULL) {
		return 0;
	}

	char *tok, *saveptr, *strl, *placeholder;
	int counter = 0;

	strtok_r(buf, "\n", &saveptr);

	for (placeholder = strl = strdup(buf); ; strl = NULL) {
		tok = strtok_r(strl, delim, &saveptr);
		if (tok == NULL) {
		       break;
		}
 		counter++;
	}

	free(placeholder);	
	return counter;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/
	command_line result;

	char* buf_copy = strdup(buf); // create a copy of buf
	char* saveptr;
	// do later: check if buf_copy is null and do something

	result.num_token = count_token(buf_copy, delim);

	// Allocate memory for array
	result.command_list = (char**)malloc((result.num_token + 1) * sizeof(char*));
	// do later: check if command_list is null and do something

	char* tok = strtok_r(buf_copy, delim, &saveptr); // I got this line straight from stack overflow
	int count = 0;

	while (tok != NULL) {
		// Allocate memory for each token and append it to array
		result.command_list[count] = strdup(tok);

		tok = strtok_r(NULL, delim, &saveptr);
		count++;
	}

	result.command_list[result.num_token] = NULL;

	free(buf_copy); // Free the buf copy

	return result;
}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/
	
	// I dont think I need to explain whats going here, but why not.
	// First I loop thru command.num_tokens to free every index of my array
	// then I free the array allocation itself.
	for (int i = 0; i < command->num_token; i++) {
		free(command->command_list[i]);
	}
	
	free(command->command_list);
}
