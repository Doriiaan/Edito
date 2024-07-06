/**
 * ===================================================
 * @file eFile.c
 * @brief Contain eFile structure and functions
 * @author ALARY Dorian
 * @version 0.1
 * @date 23/06/2024
 * @copyright GNU Public License.
 *
 * @details This file contains all the structures, variables and functions used to manage the file and its contents. 
 *          The methods allow you to :
 *              - initialize and allocate eFile from the name of a file,
 *              - modify the contents of eFile and
 *              - write the contents of eFile into the file used for initialization.
 * ===================================================
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
 * @param filename: Name of the file
 * @return PERM value defining the permission of the file.
 */
PERM file_permissions(const char *filename)
{
	errno=0;
	if(access(filename, R_OK) == -1)
	{
		if(errno==ENOENT)
		{
			return p_CREATE;
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
	memset(buffer, 0, BUFFER_LENGTH);


	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	if(efile == NULL)
	{
		return NULL;
	}
	
	efile->filename = filename;
	
	efile->n_elines = 0;

	if((efile->permissions = file_permissions(filename)) == p_NOPERM)
	{
		free(efile);
		return NULL;	
	}

	if(efile->permissions == p_CREATE)
	{
		if((fp = fopen(filename, "w+")) == NULL)
		{
			free(efile);
			return NULL;
		}
		add_empty_line_eFile(efile, 1);
		return efile;
	}

	/* open file to read it */
	else if(efile->permissions != p_CREATE && (fp = fopen(filename, "r")) == NULL)
	{
		free(efile);
		return NULL;
	}


	/* Loop to read lines of the file*/
	while(fgets(buffer, BUFFER_LENGTH, fp) != NULL)
	{
		if((current = create_eLine(buffer, BUFFER_LENGTH, efile->n_elines+1, previous, NULL)) == NULL)
		{
			delete_eFile(&efile);	
			return NULL;
		}

		/* If it is not the end of line */
		while(buffer[strlen(buffer)-1] != '\n' && fgets(buffer, BUFFER_LENGTH, fp) != NULL)
		{
			if(insert_string_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				delete_eFile(&efile);
				return NULL;
			}
		}

	   	if(efile->n_elines==0)
			efile->first = current;

		/* reinit for future lines */
		efile->n_elines++;
		previous = current;
		current = NULL;
	}

	if(efile->n_elines == 0)
	{
		add_empty_line_eFile(efile, 0);
		efile->n_elines = 1;
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
int write_eFile(eFile *efile)
{
	FILE *fp = NULL;
	eLine *current = NULL;
	char *buffer = NULL;
	size_t buffer_length = 0;

	if(efile == NULL)
	{
		return -1;
	}

	if(efile->permissions != p_READWRITE)
	{
		return -1;
	}

	if((fp = fopen(efile->filename, "w")) == NULL)
	{
		return -1;
	}

	current = efile->first;
	
	while(current)
	{
		/* Realloc buffer if necessary, first time realloc is equal to malloc */
		if(buffer_length < current->length+2)
		{
			buffer_length = sizeof(char)*(current->length+2);
			buffer = (char *) realloc(buffer, buffer_length);
		}
		
		memset(buffer, 0, buffer_length);
		memcpy(buffer, current->string, current->length);
		buffer[current->length] = '\n';

		if(fputs(buffer, fp) == EOF)
		{
			fclose(fp);
			return -1;
		}
		current=current->next;
	}

	free(buffer);
	fclose(fp);
	
	return 0;
}


/**
 * @brief the delete_eFile() delete and deallocate eFile and set the pointer to NULL.
 *
 * @param efile eFile pointer pointer
 */
void delete_eFile(eFile **efile)
{
	eLine *current = (*efile)->first, *temp=NULL;

	while(current)
	{
		temp = current->next;
		delete_eLine(&current);
		current = temp;
	}

	free(*efile);
	*efile = NULL;
}

/**
 * @brief the add_empty_line_eFile() add an empty line to the position pos in the file
 *
 * @param efile eFile pointer pointer
 * @param pos  Position of the new line in the file
 */
int add_empty_line_eFile(eFile *efile, unsigned int pos)
{
	eLine *current = efile->first;
	eLine *new = NULL;
	unsigned int i = 1;

	if(efile->first == NULL)
	{
		if((efile->first = create_eLine("", 0, 1, NULL, NULL)) == NULL)
		{
			return -1;
		}
		efile->n_elines = 1;
		return 0;
		
	}

	while(i < pos-1 && current->next)
	{
		current=current->next;
		i++;
	}
	
	/* Current is previous*/
	if((new = create_eLine("", 0, pos, current, current->next)) == NULL)
	{
		return -1;
	}
	efile->n_elines++;

	/* Increment line pos */
	current = new->next;
	while(current)
	{
		current->pos++;
		current = current->next;
	}
	return 0;
}
			

/**
 * @brief the delete_line_eFile() delete a line to the position pos in the file
 *
 * @param efile eFile pointer pointer
 * @param pos Position of the line that will be deleted
 */
void delete_line_eFile(eFile *efile, unsigned int pos)
{
	eLine *current = efile->first;
	eLine *tmp = NULL;
	unsigned int i = 1;

	while(current && i < pos)
	{
		current = current->next;
		i++;
	}

	if(current == NULL)
		return;

	if(current->next != NULL)
		current->next->previous = current->previous;

	if(current->previous != NULL)
		current->previous->next = current->next;

	/* Delete line and decrement line pos */
	tmp=current->next;
	delete_eLine(&current);
	efile->n_elines--;
	current = tmp;
	while(current)
	{
		current->pos--;
		current = current->next;
	}
}
