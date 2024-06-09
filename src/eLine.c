#include <string.h> /* strlen */
#include <stdlib.h> /* malloc */
#include "eLine.h"

static int pos = 0;

eLine *create_eLine(char *string, eLine *next, eLine *previous)
{
	struct eLine *el = (struct eLine *) malloc(sizeof(struct eLine));
	if(el == NULL)
	{
		/* Trace and error */
	}
	
	el->pos = pos++;
	el->length = strlen(string);
	el->next = next;
	el->previous = previous;

	el->string = string;

	return el;
}


