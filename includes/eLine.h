/**
 * @file eLine.h
 * @brief eLine Header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 */


#ifndef __ELINE_H__
#define __ELINE_H__

#include <stddef.h> /* size_t */


/**
 * @struct eLine structure to reprensent a line of a file in memory.
 *         Composant of a linked list.
 */
typedef struct eLine
{
    /** Length of line */
    size_t length;

    /** Allocated size */
    size_t alloc_size;

    /** Line number */
    unsigned int line_number;

    /** Previous line of NULL */
    struct eLine * previous;

    /** Next line or NULL*/
    struct eLine * next;

    /** Characters of the line, including \n character */
    char *string;

} eLine;


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
                     eLine * next);


/**
 * @brief The delete_eLine() function delete and deallocate eLine and
 *        set pointer to NULL.
 *
 * @param eline: eLine pointer pointer
 */
void delete_eLine(eLine ** eline);


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
                        unsigned int pos);


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
                        unsigned int pos);


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
                      unsigned int pos);


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
                      unsigned int pos);


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
                     unsigned int pos);

#endif
