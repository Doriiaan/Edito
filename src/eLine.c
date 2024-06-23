/**
 * @file 		eLine.c
 * @brief 		Contain eLine structure and functions
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 * @details 	This file contains all the structures, variables and functions used to manage lines. 
 * 				The functions allow you to :
 * 					- allocate and initialize an eLine,
 * 					- modify content of a eLine and
 * 					- delete and deallocate a eLine.
 */


#include <string.h> /* strnlen */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* EOF */

#include "eLine.h"


unsigned int get_next_power_of_two(unsigned int n);

	
/**
 * @brief The create_eLine() function allocate and initialize an eLine.
 *
 * @param string: 	String of line.
 * @param length: 	Length of the string (excluding null terminator)
 * @param pos: 		Position of the line in the file.
 * @param next: 	Next line in file.
 * @param previous: Previous line in file.
 *
 * @return Pointer on the line structure or NULL if allocation failed.
 *
 * @note delete_eLine() must be called before exiting.
 */
eLine *create_eLine(char *string, size_t length, unsigned int pos, eLine *previous, eLine *next)
{
	struct eLine *eline = (struct eLine *) malloc(sizeof(struct eLine));
	if(eline == NULL)
	{
		/* TODO: TRACE */
		return NULL;
	}

	eline->alloc_size = get_next_power_of_two(strnlen(string, length))*sizeof(char);
	
	eline->string = (char *) malloc(eline->alloc_size);
	if(eline->string == NULL)
	{
		/* TODO: TRACE */
		free(eline);
		return NULL;
	}

	eline->length = strnlen(string, length);
	memset(eline->string, 0, eline->alloc_size);
	memcpy(eline->string, string, eline->length);


	eline->pos = pos;

	eline->next = next;
	if(eline->next)
		eline->next->previous = eline;

	eline->previous = previous;
	if(eline->previous)
		eline->previous->next = eline;

	return eline;
}


/**
 * @brief The insert_eLine() function insert length character of the string in the line at position pos.
 *
 * @param eline: 	eLine 
 * @param string: 	The string to insert
 * @param length:	Number of character to insert from string into the eLine
 * @param pos:		Position where to insert the string
 *
 * @return 0 on success, -1 on error, see logs
 *
 */
int insert_eLine(eLine *eline, const char *string, size_t length, unsigned int pos)
{
	size_t string_length = strnlen(string, length);
	size_t new_length = eline->length + string_length;
	
	if(pos > eline->length + 1)
	{
		/* TODO: TRACE */
		return -1;
	}

	if(new_length+1 > eline->alloc_size)
	{
		eline->alloc_size = sizeof(char)*get_next_power_of_two(new_length);
		eline->string = (char *) realloc(eline->string, eline->alloc_size);
	}


	memset(eline->string + eline->length, 0, eline->alloc_size - eline->length);
	memmove(eline->string + pos + string_length, eline->string + pos, eline->length - pos);
	memcpy(eline->string+pos, string, string_length);

	eline->length = new_length;

	return 0;
}


/**
 * @brief The delete_eLine() function delete and deallocate eLine and set pointer to NULL
 *
 * @param:	eline pointer pointer
 */
void delete_eLine(eLine **eline)
{
	if(*eline != NULL)
	{
		if((*eline)->string != NULL)
			free((*eline)->string);
		free(*eline);
		*eline = NULL;
	}
}


/*
 * @brief The get_next_power_of_two() function is an intern function to calculate the next power of two after a number n.
 *
 * @example get_next_power_of_two(7) -> 8
 * @example get_next_power_of_two(16) -> 32
 * @example get_next_power_of_two(0) -> 1
 */
unsigned int get_next_power_of_two(unsigned int n)
{
	return (1 << (sizeof(unsigned int)*8 - __builtin_clz(n)));
}
