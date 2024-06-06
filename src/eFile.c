#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */
#include <errno.h> /* perror, errno, ferror */
#include "eFile.h" 

#define POSIX_LENGTH_MAX_LINE 4096

eFile* open_eFile(const char *filename, const char *mode)
{
	FILE *fp; 
	eFile *efile;
	eLine *eLine;
   	char *line; /* final string to create eLine */
	char buffer[POSIX_LENGTH_MAX_LINE]; /* buffer to create line */
	size_t buffer_length;

	/* Open folder */
	fp = fopen(filename, mode);
	if(fp == NULL)
	{
		/* TRACE */
	}
	
	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	efile->fp = fp;

	/* Read and allocate lines */
	while(fgets(buffer, POSIX_LENGTH_MAX_LINE, fp) != NULL)
	{
		buffer_length = strlen(buffer);
		
		/* Allocate one line */
		line = (char *) malloc(sizeof(char)*(buffer_length+1));
		strncpy(line, buffer, buffer_length);

		if(buffer[buffer_length]-1 != '\n') /* buffer do not contain the entire line */
		{
			/* 
			 * Lire le fichier jusqu a trouver un \n
			 */

		}
	}

	return efile;
}

int write_eFile(eFile *ef);
void read_eFile(eFile *ef);
void close_eFile(eFile *ef);

