#include <stdio.h> /* printf */
#include <string.h> /* strlen */
#include <stdlib.h> /* malloc, error */
#include "eLine.h"


eLine *create_eLine(char *string, int pos, eLine *next, eLine *previous)
{
	struct eLine *el = (struct eLine *) malloc(sizeof(struct eLine));
	if(el == NULL)
	{
		perror("Error when allocating eLine memory");
	}
	
	el->pos = pos;
	el->length = strlen(string);
	el->next = next;
	el->previous = previous;

	el->string = string;

	return el;
}


