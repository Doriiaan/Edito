/**
 * @file eFile.c
 * @brief Contain eFile structure and functions
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 * @details This file contains all the structures, variables and functions
 *          used to manage the file and its content.
 */

#include "eFile.h"
#include "eLine.h"
#include "util.h"

#include <stdio.h> /* printf, FILE */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen, strncpy, strncat */
#include <errno.h> /* errno code */
#include <unistd.h> /* access */
#include <stdbool.h>


#define BUFFER_LENGTH 256 /* Buffer length of the buffer used to read lines */


/**
 * @brief The file_permissions() function return the permission of the file
 *        designed by realpath.
 *
 * @param realpath: Name of the file
 *
 * @return PERM value defining the permission of the file.
 */
PERM file_permissions(char const * realpath)
{
    errno=0;
    if(access(realpath, R_OK) == -1)
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
        if(access(realpath, W_OK) == -1)
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
 * @brief The create_eFile() function allocate an eFile structure but do
 *        not open or allocate lines.
 *
 * @param realpath: Name of the file
 *
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile * create_eFile(char const * realpath)
{
    eFile *efile=NULL;

    /* Allocate and fill the data structure */
    efile = (eFile *) malloc(sizeof(eFile));
    if(efile == NULL)
    {
        return NULL;
    }

    efile->permissions = file_permissions(realpath);

    efile->realpath = strdup(realpath);

    if(efile->realpath[strlen(efile->realpath)-1] == '/')
        efile->realpath[strlen(efile->realpath)-1] = 0;

    efile->filename = strrchr(efile->realpath, '/');
    efile->filename = (efile->filename != NULL) ? efile->filename+1
                                                : efile->realpath;

    efile->n_elines = 0;
    efile->first_file_line = NULL;
    efile->first_screen_line = NULL;
    efile->current_line = NULL;
    efile->current_pos = 0;
    efile->is_saved = true;

    return efile;
}


/**
 * @brief The delete_eFile() delete and deallocate eFile and set the
 *        pointer to NULL.
 *
 * @param efile: eFile pointer pointer
 */
void delete_eFile(eFile ** efile)
{
    if(*efile == NULL)
        return;

    close_eFile(*efile);
    free((*efile)->realpath);
    free(*efile);
    *efile = NULL;
}


/**
 * @brief The open_eFile() function open the file and initialize lines.
 *
 * @param efile: eFile pointer
 *
 * @note close_eFile() must be called to deallocate lines. Delete_eFile()
 *       also call close_eFile(). If the file did not exist when calling
 *       create_eFile(), open_eFile write a new file.
 */
int open_eFile(eFile * efile)
{
    eLine *current = NULL, *previous = NULL;
    FILE *fp = NULL;
    char buffer[BUFFER_LENGTH];
    memset(buffer, 0, BUFFER_LENGTH);

    if(efile == NULL || efile->permissions == p_NOPERM)
        return -1;

    /* If the file did not exist when calling create_eFile() */
    if(efile->permissions == p_CREATE)
    {
        if((fp = fopen(efile->realpath, "w+")) == NULL)
        {
            return -1;
        }

        add_empty_line_eFile(efile, 1);
        efile->permissions = p_READWRITE;
        return 0;
    }

    /* Open file to read it */
    else if(efile->permissions != p_CREATE)
    {
        if((fp = fopen(efile->realpath, "r")) == NULL)
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
 * @brief The close_eFile() function close the file and deallocate eLines.
 *
 */
void close_eFile(eFile *efile)
{
    if(efile == NULL)
        return;

    eLine *current = efile->first_file_line, *temp=NULL;

    while(current)
    {
        temp = current->next;
        delete_eLine(&current);
        current = temp;
    }

    efile->n_elines = 0;
    efile->first_file_line = NULL;
    efile->first_screen_line = NULL;
    efile->current_line = NULL;
    efile->current_pos = 0;
    efile->is_saved = true;
}


/**
 * @brief The write_eFile() function write the content of eFile on the
 *        file designed by filename stored in the filename attribute.
 *
 * @param efile eFile pointer
 *
 * @return 0 on sucess or -1 in failure.
 */
int write_eFile(eFile *efile)
{
    FILE *fp = NULL;
    eLine *current = NULL;
    char *buffer = NULL;
    size_t buffer_length = 0;

    if(efile == NULL)
        return -1;

    if(efile->is_saved)
        return 0;

    if(efile->permissions != p_READWRITE)
        return -1;

    if((fp = fopen(efile->realpath, "w")) == NULL)
        return -1;

    current = efile->first_file_line;

    while(current)
    {
        /* Realloc buffer if necessary, first time realloc is equal
           to malloc */
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
 * @brief The add_empty_line_eFile() add an empty line to the position
 *        pos in the file.
 *
 * @param efile: eFile pointer pointer
 * @param line number: Line number of the new line in the file
 *
 * @return 0 on success or -1 in failure.
 */
int add_empty_line_eFile(eFile * efile,
                         unsigned int line_number)
{
    eLine *current = NULL;
    eLine *new = NULL;
    unsigned int i = 1;

    if(efile == NULL)
        return -1;

    current = efile->first_file_line;

    if(efile->first_file_line == NULL)
    {
        efile->first_file_line = create_eLine("", 0, 1, NULL, NULL);
        if(efile->first_file_line == NULL)
        {
            return -1;
        }

        efile->n_elines = 1;
        efile->is_saved = false;
        return 0;

    }

    while(i < line_number-1 && current->next)
    {
        current=current->next;
        i++;
    }

    /* Current is previous*/
    if((new = create_eLine("", 0, line_number, current, current->next)) == NULL)
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
 * @brief The delete_line_eFile() delete a line to the position pos in
 *        the file.
 *
 * @param efile: eFile pointer pointer
 * @param line_number: Line number of the line that will be deleted
 *
 * @return 0 on sucess or -1 in failure.
 */
int delete_line_eFile(eFile * efile,
                      unsigned int line_number)
{
    eLine *current = NULL;
    eLine *tmp = NULL;
    unsigned int i = 1;
    bool last_line=false;

    if(efile == NULL)
        return -1;

    current = efile->first_file_line;

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
        return -1;

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

    return 0;
}


/**
 * @brief The insert_char_eFile() insert a character to the current
 *        position in the current_line.
 *
 * @param efile: eFile pointer
 * @param ch: character to insert
 *
 * @return 0 on sucess or -1 in failure.
 */
int insert_char_eFile(eFile * efile,
                      char const ch)
{
    if(efile == NULL)
        return -1;

    if(insert_char_eLine(efile->current_line, ch, efile->current_pos))
        return -1;

    efile->is_saved = false;
    return 0;
}


/**
 * @brief The remove_char_eFile() remove a character to the current
 *        position in the current_line.
 *
 * @param efile: eFile pointer
 *
 * @return 0 on sucess or -1 in failure.
 */
int remove_char_eFile(eFile * efile)
{
    if(efile == NULL)
        return -1;

    if(remove_char_eLine(efile->current_line, efile->current_pos))
        return -1;

    efile->is_saved = false;
    return 0;
}


/**
 * @brief The insert_string_eFile() insert the string at current_pos in
 *        the current line.
 *
 * @param efile: eFile pointer
 * @param string: string to insert
 * @param length: lenght of string
 *
 * @return 0 on sucess or -1 in failure.
 */
int insert_string_eFile(eFile * efile,
                        char const * string,
                        size_t length)
{
    int result = 0;
    if(efile == NULL)
        return -1;

    result = insert_string_eLine(efile->current_line,
                                 string,
                                 length,
                                 efile->current_pos);
    if(result == -1)
    {
        return -1;
    }

    efile->is_saved = false;
    return 0;
}


/**
 * @brief The remove_string_eFile() remove the string that start at
 *        current_pos and end to current_pos+length in the current line.
 *
 * @param efile: eFile pointer
 * @param length: of string to remove
 *
 * @return 0 on sucess or -1 in failure.
 */
int remove_string_eFile(eFile *efile,
                        size_t length)
{
    int result = 0;

    if(efile == NULL)
        return -1;

    result = remove_string_eLine(efile->current_line,
                                 length,
                                 efile->current_pos);
    if(result)
    {
        return -1;
    }

    efile->is_saved = false;
    return 0;
}
