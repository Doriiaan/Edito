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

#include "eFile.h"
#include "eLine.h"

#include <stdio.h> /* printf, FILE */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen, strncpy, strncat */
#include <errno.h> /* errno code */
#include <unistd.h> /* access */
#include <stdbool.h>


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
 * @brief The create_eFile() function allocate an eFile structure but do not open or allocate lines.
 *
 * @param filename:	Name of the file
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile* create_eFile(char *filename)
{
	eFile *efile=NULL;

	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	if(efile == NULL)
	{
		return NULL;
	}
	
	if((efile->permissions = file_permissions(filename)) == p_NOPERM)
	{
		free(efile);
		return NULL;	
	}

	efile->filename = filename;
	efile->n_elines = 0;
	efile->first_file_line = NULL;
	efile->first_screen_line = NULL;
	efile->current_line = NULL;
	efile->current_pos = 0;
	efile->is_saved = true;

	return efile;
}


/**
 * @brief The open_eFile() function open the file and initialize lines.
 *
 * @note close_eFile() must be called to deallocate lines, delete_eFile also call close_eFile().
 * @note If the file did not exist when calling create_eFile(), open_eFile write a new file.
 */
int open_eFile(eFile *efile)
{
	eLine *current = NULL, *previous = NULL;
	FILE *fp = NULL;
	char buffer[BUFFER_LENGTH];
	memset(buffer, 0, BUFFER_LENGTH);	

	/* If the file did not exist when callinf create_eFile() */
	if(efile->permissions == p_CREATE)
	{
		if((fp = fopen(efile->filename, "w+")) == NULL)
		{
			return -1;
		}

		add_empty_line_eFile(efile, 1);
		return 0; 
	}

	/* Open file to read it */
	else if(efile->permissions != p_CREATE && (fp = fopen(efile->filename, "r")) == NULL)
	{
		return -1;
	}


	/* Loop to read lines of the file*/
	while(fgets(buffer, BUFFER_LENGTH, fp) != NULL)
	{
		if((current = create_eLine(buffer, BUFFER_LENGTH, efile->n_elines+1, previous, NULL)) == NULL)
		{
			close_eFile(efile);	
			fclose(fp);
			return -1;
		}

		/* If it is not the end of line */
		while(buffer[strlen(buffer)-1] != '\n' && fgets(buffer, BUFFER_LENGTH, fp) != NULL)
		{
			if(insert_string_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				close_eFile(efile);
				fclose(fp);
				return -1;
			}
		}

	   	if(efile->n_elines==0)
			efile->first_file_line = current;

		/* Reinit for future lines */
		efile->n_elines++;
		previous = current;
		current = NULL;
	}

	if(efile->n_elines == 0)
	{
		add_empty_line_eFile(efile, 0);
		efile->n_elines = 1;
	}

	efile->current_line = efile->first_file_line;
	efile->current_pos = 0;
	efile->first_screen_line = efile->first_file_line;
	efile->is_saved = true;

	fclose(fp);
	return 0;
}


/**
 * @brief The close_eFile() function close the file and delete lines.
 *
 */
void close_eFile(eFile *efile)
{
	efile->current_line = NULL;
	efile->current_pos = 0;
	efile->first_screen_line = NULL;

	eLine *current = efile->first_file_line, *temp=NULL;

	while(current)
	{
		temp = current->next;
		delete_eLine(&current);
		current = temp;
	}
	efile->is_saved = true;
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

	current = efile->first_file_line;
	
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

	efile->is_saved = true;
	
	return 0;
}


/**
 * @brief the delete_eFile() delete and deallocate eFile and set the pointer to NULL.
 *
 * @param efile eFile pointer pointer
 */
void delete_eFile(eFile **efile)
{
	close_eFile(*efile);
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
	eLine *current = efile->first_file_line;
	eLine *new = NULL;
	unsigned int i = 1;

	if(efile->first_file_line == NULL)
	{
		if((efile->first_file_line = create_eLine("", 0, 1, NULL, NULL)) == NULL)
		{
			return -1;
		}
		
		efile->n_elines = 1;
		efile->is_saved = false;
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

	/* Increment line number */
	current = new->next;
	while(current)
	{
		current->line_number++;
		current = current->next;
	}
	
	efile->is_saved = false;
	return 0;
}
			

/**
 * @brief the delete_line_eFile() delete a line to the position pos in the file
 *
 * @param efile eFile pointer pointer
 * @param line_number Position of the line that will be deleted
 */
void delete_line_eFile(eFile *efile, unsigned int line_number)
{
	eLine *current = efile->first_file_line;
	eLine *tmp = NULL;
	unsigned int i = 1;
	bool last_line=false;

	if(line_number == efile->current_line->line_number)
	{
		if(efile->current_line->next)
			efile->current_line = efile->current_line->next;

		else if(efile->current_line->previous)
			efile->current_line = efile->current_line->previous;
		else
			last_line = true;
	}
	
	if(line_number == efile->first_screen_line->line_number)
	{
		if(efile->first_screen_line->next)
			efile->first_screen_line = efile->first_screen_line->next;

		else if(efile->first_screen_line->previous)
			efile->first_screen_line = efile->first_screen_line->previous;
		else
			last_line = true;
	}
	
	if(last_line)
		add_empty_line_eFile(efile, line_number+1);

	
	if(line_number == efile->first_file_line->line_number)
		efile->first_file_line = efile->first_file_line->next;

	while(current && i < line_number)
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

	/* Delete line and decrement line number */
	tmp=current->next;
	delete_eLine(&current);
	efile->n_elines--;
	current = tmp;
	while(current)
	{
		current->line_number--;
		current = current->next;
	}

	efile->is_saved = false;
}


/**
 * @brief the insert_char_eFile() insert a character to the current position in the current_line 
 *
 * @param efile eFile pointer
 * @param ch character to insert
 */
void insert_char_eFile(eFile *efile, const char ch)
{
	if(!insert_char_eLine(efile->current_line, ch, efile->current_pos))
		efile->is_saved = false;	
}


/**
 * @brief the remove_char_eFile() remove a character to the current position in the current_line 
 *
 * @param efile eFile pointer 
 */
void remove_char_eFile(eFile *efile)
{
	if(!remove_char_eLine(efile->current_line, efile->current_pos))
		efile->is_saved = false;	
}


/**
 * @brief the insert_string_eFile() insert the string at current_pos in the current line 
 *
 * @param efile eFile pointer
 * @param string string to insert
 * @param length lenght of string
 */
void insert_string_eFile(eFile *efile, const char *string, size_t length)
{
	if(!insert_string_eLine(efile->current_line, string, length, efile->current_pos))
		efile->is_saved = false;
}


/**
 * @brief the remove_string_eFile() remove the string that start at current_pos and end to current_pos+length in the current line 
 *
 * @param efile eFile pointer
 * @param length of string to remove
 */
void remove_string_eFile(eFile *efile, size_t length)
{
	if(!remove_string_eLine(efile->current_line, length, efile->current_pos))
		efile->is_saved = false;	
}
