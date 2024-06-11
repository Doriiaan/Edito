#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <errno.h> /* perror, errno, ferror */
#include <string.h> /* strlen, strncpy, strncat */
#include "eFile.h" 

#define BUFFER_LENGTH 4096 

char *dynamic_fgets(FILE *fp);


eFile* open_eFile(const char *filename, const char *mode)
{
	FILE *fp; 
	eFile *efile;
	eLine *current, *previous=NULL;
	char buffer[BUFFER_LENGTH];
	int pos = 1;


	/* Open file */
	fp = fopen(filename, mode);
	if(fp == NULL)
	{
		/* TODO: TRACE */
		return NULL;
	}


	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	efile->fp = fp;


	/* Loop to read lines of the file*/	
	while(fgets(buffer, BUFFER_LENGTH, efile->fp) != NULL)
	{
		current = create_eLine(buffer, BUFFER_LENGTH, pos, previous, NULL);
		if(current == NULL)
		{
			/* TODO: TRACE */
			return NULL;
		}
		
		/* If it is not the end of line (the second test is here in the case the last line has not \n) */
		while(buffer[strlen(buffer)-1] != '\n' && fgets(buffer, BUFFER_LENGTH, efile->fp) != NULL) 
		{
			if(insert_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				/* TODO: TRACE */
				break;
			}
		}

		if(get_char_eLine(current, get_length_eLine(current)-1)!= '\n')
			if(insert_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				/* TODO: TRACE */
			}

	   	if(pos==1)
			efile->first = current;


		if(current->previous != NULL)
			current->previous->next = current;


		/* reinit for future lines */
		pos++;
		previous = current;
		current = NULL;
	}

	efile->n_elines = pos-1;
	
	return efile;
}

int write_eFile(eFile *ef);
void close_eFile(eFile *ef);
