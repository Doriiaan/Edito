/**
 * @file 		eFile.c
 * @brief 		Contain eFile structure and functions
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 * @details 	This file contains all the structures, variables and functions used to manage the file and its contents. 
 * 				The methods allow you to :
 * 					- initialize and allocate eFile from the name of a file,
 * 					- modify the contents of eFile and
 * 					- write the contents of eFile into the file used for initialization.
 */

#include <stdio.h> /* printf, FILE */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen, strncpy, strncat */
#include <errno.h> /* errno code */
#include <unistd.h> /* access */

#include "eFile.h"
#include "eLine.h"

#define BUFFER_LENGTH 256 /* Buffer length of the buffer used to read lines */


/**
 * @brief The file_permissions() function return the permission of the file designed by filename.
 *
 * @param filename:	Name of the file
 * @return PERM value defining the permission of the file.
 */
PERM file_permissions(const char *filename)
{
	errno=0;
	if(access(filename, R_OK) == -1)
	{
		if(errno==ENOENT)
		{
			return p_READWRITE;
		}
		else
		{
			return p_NOPERM;
		}
	}
	else
	{
		if(access(filename, W_OK) == -1)
		{
			return p_READONLY;
		}
		else
		{
			return p_READWRITE;
		}
	}
}


/**
 * @brief The create_eFile() function allocate and initialize an eFile structure.
 *
 * @param filename:	Name of the file
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile* create_eFile(const char *filename)
{
	FILE *fp=NULL;
	eFile *efile=NULL;
	eLine *current=NULL, *previous=NULL;
	char buffer[BUFFER_LENGTH];


	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	if(efile == NULL)
	{
		/* TODO:TRACE */
		return NULL;
	}


	if((efile->permissions = file_permissions(filename)) == p_NOPERM)
	{
		/* TODO:TRACE */
		free(efile);
		return NULL;	
	}


	/* open file to read it */
	if((fp = fopen(filename, "r")) == NULL)
	{
		/* TODO: TRACE */
		free(efile);
		return NULL;
	}


	efile->n_elines = 0;
	efile->filename = filename;


	/* Loop to read lines of the file*/
	while(fgets(buffer, BUFFER_LENGTH, fp) != NULL)
	{
		if((current = create_eLine(buffer, BUFFER_LENGTH, efile->n_elines+1, previous, NULL)) == NULL)
		{
			/* TODO: TRACE */
			delete_eFile(&efile);	
			return NULL;
		}


		/* If it is not the end of line */
		while(buffer[strlen(buffer)-1] != '\n' && fgets(buffer, BUFFER_LENGTH, fp) != NULL)
		{
			if(insert_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				/* TODO: TRACE */
				delete_eFile(&efile);
				return NULL;
			}
		}


		/* If the last line does not have a \n */
		if(current->string[current->length-1] != '\n')
			if(insert_eLine(current, "\n", 1, current->length))
			{
				/* TODO: TRACE */
				delete_eFile(&efile);
				return NULL;
			}

	   	if(efile->n_elines==0)
			efile->first = current;


		/* reinit for future lines */
		efile->n_elines++;
		previous = current;
		current = NULL;
	}

	fclose(fp);
	return efile;
}


/**
 * @brief The write_eFile() function write the content of eFile on the file designed by filename stored in the filename attribute.
 *
 * @param efile eFile pointer
 * @return 0 on sucess or -1 in failure, see logs.
 */
int write_eFile(eFile *efile);


/**
 * @brief the delete_Efile() delete and deallocate eFile and set the pointer to NULL.
 *
 * @param efile eFile pointer pointer
 */
void delete_eFile(eFile **efile)
{
	eLine *current = (*efile)->first, *temp=NULL;

	while(current->next)
	{
		temp = current->next;
		delete_eLine(&current);
		current = temp;
	}

	free(*efile);
	*efile = NULL;
}
