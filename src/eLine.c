#include <string.h> /* strnlen */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* EOF */

#include "eLine.h"

unsigned int get_next_power_of_two(unsigned int n);

	
/*
 * @brief Allocate and complete eLine structure
 *
 * @param [IN] char *string: string of line.
 * @param [IN] size_t length: length of the string (excluding null terminator)
 * @param [IN] unsigned int pos: position of the line in the file.
 * @param [IN] eLine *next: next line in file.
 * @param [IN] eLine *previous: previous line in file.
 *
 * @return eLine *: pointer on the line structure or NULL if allocation failed.
 *
 * @note delete_eLine() must be called before exiting
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
	eline->previous = previous;

	return eline;
}


/* GETTERs AND SETTERs */
size_t get_length_eLine(const eLine *eline)
{
	return eline->length;
}

unsigned int get_pos_eLine(const eLine *eline)
{
	return eline->pos;
}

eLine *get_next_eLine(const eLine *eline)
{
	return eline->next;
}

int get_char_eLine(const eLine *eline, size_t i)
{
	if(i < eline->length)
		return (eline->string)[i];
	else
		return EOF;
}

eLine *get_previous_eLine(const eLine *eline)
{
	return eline->previous;
}

void set_next_eLine(eLine *eline, eLine *next)
{
	eline->next = next;
}

void set_previous_eLine(eLine *eline, eLine *previous)
{
	eline->previous = previous;
}


/*
 *
 *
 *
 */
int insert_eLine(eLine *eline, const char *string, size_t length, unsigned int pos)
{
	size_t string_length = strnlen(string, length);
	size_t new_length = eline->length + string_length;
	
	if(pos > eline->length + 1)
		return -1;

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

unsigned int get_next_power_of_two(unsigned int n)
{
	return (1 << (sizeof(unsigned int)*8 - __builtin_clz(n)));
}
