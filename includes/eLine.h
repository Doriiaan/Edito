/**
 * @file 		eLine.h
 * @brief 		eLine Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */


#ifndef __ELINE_H__
#define __ELINE_H__

#include <stddef.h> /* size_t */


/**
 * @struct eLine structure to reprensent a line of a file in memory. Composant of a linked list.
 */
typedef struct eLine
{
	/** Length of line */
	size_t length;

	/** Allocated size */
	size_t alloc_size;	
	
	/** Position of the line in the file */
	unsigned int pos;
	
	/** Previous line of NULL */
	struct eLine *previous; 
	
	/** Next line or NULL*/
	struct eLine *next; 
	
	/** Characters of the line, including \n character */
	char *string; 

} eLine;


/**
 * @brief The create_eLine() function allocate and initialize an eLine.
 *
 * @param string: 	String of line.
 * @param length: 	Length of the string (excluding null terminator)
 * @param pos: 		Position of the line in the file.
 * @param next: 	Next line in file.
 * @param previous: Previous line in file.
 *
 * @return Pointer on the line structure or NULL if allocation failed.
 *
 * @note delete_eLine() must be called before exiting.
 */
eLine *create_eLine(char *string, size_t length, unsigned int pos, eLine *previous, eLine *next);


/**
 * @brief The insert_eLine() function insert length character of the string in the line at position pos.
 *
 * @param eline: 	eLine 
 * @param string: 	The string to insert
 * @param length:	Number of character to insert from string into the eLine
 * @param pos:		Position where to insert the string
 *
 * @return 0 on success, -1 on error, see logs
 *
 */
int insert_eLine(eLine *eline, const char *string, size_t length, unsigned int pos);


/**
 * @brief The delete_eLine() function delete and deallocate eLine and set pointer to NULL
 *
 * @param:	eline pointer pointer
 */
void delete_eLine(eLine **eline);

#endif
