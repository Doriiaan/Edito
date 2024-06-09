#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <errno.h> /* perror, errno, ferror */
#include <string.h> /* strlen, strncpy, strncat */
#include "eFile.h" 

#define POSIX_LENGTH_MAX_LINE 10 

char *dynamic_fgets(FILE *fp);


eFile* open_eFile(const char *filename, const char *mode)
{
	FILE *fp; 
	eFile *efile;
	eLine *eline, *previous=NULL;
   	char *string; /* final string to create eLine */
	int pos = 0; /* pos of the line in the file */


	/* Open file */
	fp = fopen(filename, mode);
	if(fp == NULL)
	{
		/* TRACE and exit */
		return NULL;
	}


	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	efile->fp = fp;


	/* Loop to read lines of the file*/	
	while((string = dynamic_fgets(efile->fp)) != NULL)
	{
		eline = (eLine *) malloc(sizeof(eLine));
	   	if(pos==0)
			efile->first = eline;


		/* Fill the eline variable */	
		eline->string = string;
		eline->length = strlen(string);
		eline->pos = pos;
		eline->previous = previous;
		eline->next = NULL;
		if(eline->previous != NULL)
			eline->previous->next = eline;


		/* reinit for future lines */
		pos++;
		previous = eline;
		eline = NULL;
	}

	efile->n_elines = pos;
	
	return efile;
}

int write_eFile(eFile *ef);
void read_eFile(eFile *ef);
void close_eFile(eFile *ef);


char *dynamic_fgets(FILE *fp)
{
	char *string;
	char length;
	char buffer[POSIX_LENGTH_MAX_LINE]; /* buffer to read line */
	size_t buffer_length;


	if(fgets(buffer, POSIX_LENGTH_MAX_LINE, fp) == NULL) /* READ size-1 and store \0 */
		return NULL;	
	
	buffer_length = strlen(buffer)+1; /* length with \0 */


	/* Allocate the string of the line */
	string = (char *) malloc(sizeof(char)*(buffer_length));
	strncpy(string, buffer, buffer_length); /* Store \0 */
	length = buffer_length;

	/* buffer do not contain the entire line */
	while(buffer[buffer_length-2] != '\n' && (fgets(buffer, POSIX_LENGTH_MAX_LINE, fp) != NULL))
	{

		buffer_length = strlen(buffer); /* there is already a \0 in string so for refactoring we do not count \0 of buffer_length */


		/* Realloc string */ 
		string = (char *) realloc(string, sizeof(char)*(length + buffer_length));
		strncat(string, buffer, buffer_length); /* size(string) must == strlen(string) + strlen(buffer) + 1 */
		length += buffer_length; /* */
	}

	return string;
}
