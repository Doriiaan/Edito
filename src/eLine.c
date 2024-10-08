/**
 * @file eLine.c
 * @brief Contain eLine structure and functions
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 * @details This file contains all the structures, variables and functions
 *          used to manage lines.
 */

#include "eLine.h"
#include "util.h"

#include <string.h> /* strnlen */
#include <stdlib.h> /* malloc */
#include <stdio.h> /* EOF */


/**
 * @brief The create_eLine() function allocate and initialize an eLine.
 *
 * @param string: String of line
 * @param length: Length of the string (excluding null terminator)
 * @param line_number: Line number
 * @param next: Next line in file
 * @param previous: Previous line in file
 *
 * @return Pointer on the line structure or NULL if allocation failed.
 *
 * @note delete_eLine() must be called before exiting.
 */
eLine * create_eLine(char const * string,
                     size_t length,
                     unsigned int line_number,
                     eLine * previous,
                     eLine * next)
{
    eLine *eline = (eLine *) malloc(sizeof(eLine));
    if(eline == NULL)
    {
        return NULL;
    }

    /* Remove '\n' */
    if(string[strnlen(string, length)-1] == '\n')
    {
        length = strnlen(string, length)-1;
    }

    eline->length = strnlen(string, length);
    eline->alloc_size = get_next_power_of_two(eline->length)*sizeof(char);

    eline->string = (char *) malloc(eline->alloc_size);
    if(eline->string == NULL)
    {
        free(eline);
        return NULL;
    }


    memset(eline->string, 0, eline->alloc_size);
    memcpy(eline->string, string, eline->length);

    eline->line_number = line_number;

    eline->next = next;
    if(eline->next)
        eline->next->previous = eline;

    eline->previous = previous;
    if(eline->previous)
        eline->previous->next = eline;

    return eline;
}


/**
 * @brief The delete_eLine() function delete and deallocate eLine and
 *        set pointer to NULL.
 *
 * @param eline: eLine pointer pointer
 */
void delete_eLine(eLine ** eline)
{
    if(*eline == NULL)
        return;

    if((*eline)->string != NULL)
        free((*eline)->string);

    free(*eline);
    *eline = NULL;
}


/**
 * @brief The insert_string_eLine() function insert length character of
 *        the string in the line at position pos.
 *
 * @param eline: eLine
 * @param string: The string to insert
 * @param length: Number of character to insert from string into the eLine
 * @param pos: Position where to insert the string
 *
 * @return 0 on success, -1 in failure.
 */
int insert_string_eLine(eLine * eline,
                        char const * string,
                        size_t length,
                        unsigned int pos)
{
    size_t string_length = 0;
    size_t new_length = 0;

    if(eline == NULL)
        return -1;

    /* del terminating \n character */
    if(string[strnlen(string, length)-1] == '\n')
    {
        length = strnlen(string, length)-1;
    }

    string_length = strnlen(string, length);
    new_length = eline->length + string_length;

    if(pos > eline->length)
    {
        return -1;
    }


    if(new_length+1 > eline->alloc_size)
    {
        eline->alloc_size = sizeof(char)*get_next_power_of_two(new_length);
        eline->string = (char *) realloc(eline->string, eline->alloc_size);
        memset(eline->string + eline->length,
               0,
               eline->alloc_size - eline->length);
    }

    memmove(eline->string + pos + string_length,
            eline->string + pos,
            eline->length - pos);
    memcpy(eline->string+pos, string, string_length);

    eline->length = new_length;

    return 0;
}


/**
 * @brief The remove_string_eLine() function remove length character of the
 *        line at position pos.
 *
 * @param eline: eLine
 * @param length: Number of character to remove from the eLine
 * @param pos: Position where to delete the string
 *
 * @return 0 on success, -1 in failure.
 */
int remove_string_eLine(eLine * eline,
                        size_t length,
                        unsigned int pos)
{
    int real_length = 0;

    if(eline ==NULL)
        return -1;

    if(pos > eline->length)
        return -1;

    /* If pos + length > eline->length*/
    real_length = strnlen(eline->string+pos, length);

    /* This move final 0 */
    memmove(eline->string+pos,
            eline->string + pos + real_length,
            eline->length-real_length-pos+1);
    eline->length -= real_length;
    return 0;
}


/**
 * @brief The insert_char_eLine() function insert a character in the line
 *        at position pos.
 *
 * @param eline: eLine
 * @param ch: The character to insert
 * @param pos: Position where to insert the string
 *
 * @return 0 on success, -1 in failure.
 */
int insert_char_eLine(eLine * eline,
                      const char ch,
                      unsigned int pos)
{
    if(eline == NULL)
        return -1;

    if(ch == '\n')
        return -1;

    if(pos > eline->length)
    {
        return -1;
    }

    eline->length ++;
    if(eline->length+1 > eline->alloc_size)
    {
        eline->alloc_size = sizeof(char)*get_next_power_of_two(eline->length);
        eline->string = (char *) realloc(eline->string, eline->alloc_size);
        memset(eline->string + eline->length,
               0,
               eline->alloc_size - eline->length);
    }

    memmove(eline->string + pos + 1, eline->string + pos, eline->length - pos);
    eline->string[pos] = ch;
    return 0;
}


/**
 * @brief The remove_char_eLine() function remove a character in the line
 *        at position pos.
 *
 * @param eline: eLine
 * @param pos: Position where to delete the remove
 *
 * @return 0 on success, -1 in failure.
 */
int remove_char_eLine(eLine * eline,
                      unsigned int pos)
{
    if(eline == NULL)
        return -1;

    if(pos > eline->length)
    {
        return -1;
    }

    if(eline->length == 0)
        return 0;

    /* This move final 0 */
    memmove(eline->string+pos, eline->string + pos + 1, eline->length - pos);
    eline->length--;
    return 0;
}


/**
 * @brief The get_string_eLine() function get a string in the line at position
 *        pos and return how many character actually got.
 *
 * @param eline: eLine
 * @param buffer: buffer where string of line is copied
 * @param length: length of buffer
 * @param pos: Position where to remove the character
 *
 * @return 0 on success, -1 in failure.
 *
 * @note : This function has security and get the min between
 *         strnlen(string, length) and eline->length character.
 */
int get_string_eLine(eLine const * eline,
                     char * buffer,
                     size_t length,
                     unsigned int pos)
{
    size_t min = 0;

    if(eline == NULL)
        return -1;

    min = (length < eline->length-pos) ? length : eline->length-pos;
    memcpy(buffer, eline->string+pos, min);
    buffer[min] = 0;

    return min;
}
