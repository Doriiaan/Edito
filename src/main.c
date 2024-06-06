/*
 * Edito main file
 *
 * ------------------------------------------------
 * Version : 0.0.0.0
 * Date : 01/06/2024
 * Brief : Creation of the file
 * -------------------------------------------------
 *
 *
 *
 */

#include <stdio.h>

#include "eLine.h" 
#include "eFile.h" 

#define FILENAME "/root/"



int main()
{

	eFile *ef = open_eFile(FILENAME, "a+");
	eLine *el = create_eLine("Hello world!\n", 0, NULL, NULL); 

	printf("eLine - address: %p\n", (void *)el);
	printf("eLine - string: %s\n", el->string);
	printf("leLine - length: %d\n", el->length);
	printf("peLine - pos: %d\n", el->pos);
	printf("neLine - next: %p\n", (void*)el->next);
	printf("peLine - previous: %p\n", (void*)el->previous);
	
	printf("\n\n");
	printf("Efile - FILE: %p\n", (void*)ef->fp);
	printf("Efile - elines: %p\n", (void*)ef->elines);
	printf("Efile - n_elines: %d\n", ef->n_elines);

	return 0;
}
