/**
 * @file 		eFile.h
 * @brief 		eFile Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __EFILE_H__
#define __EFILE_H__

#include "eLine.h"
#include "util.h"
#include <stdbool.h>


/**
 * @struct eFile structure to reprensent a file in memory 
 */
typedef struct efile_s
{
	/** File permissions */
	PERM permissions;

	/** Number of line */
	unsigned int n_elines;
	
	/** Current pos in current line */
	unsigned int current_pos;

	/** First line of eLine linked list */
	eLine *first_file_line;

	/** First line of screen */
	eLine *first_screen_line;

	/** Current line */
	eLine *current_line;

	/** Filename */
	char *filename;

	char *path;
	
	char *realpath;

	/** boolean to track status of file */
	bool is_saved;
	
} eFile;


/**
 * @brief The create_eFile() function allocate an eFile structure but do not open or allocate lines.
 *
 * @param filename:	Name of the file
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile* create_eFile(char *filename);


/**
 * @brief The open_eFile() function open the file and initialize lines.
 *
 * @note close_eFile() must be called to deallocate lines, delete_eFile also call close_eFile().
 * @note If the file did not exist when calling create_eFile(), open_eFile write a new file.
 */
int open_eFile(eFile *efile);


/**
 * @brief The close_eFile() function close the file and delete lines.
 *
 */
void close_eFile(eFile *efile);


/**
 * @brief The write_eFile() function write the content of eFile on the file designed by filename stored in the filename attribute.
 *
 * @param efile eFile pointer
 * @return 0 on sucess or -1 in failure, see logs.
 */
int write_eFile(eFile *efile);


/**
 * @brief the delete_eFile() delete and deallocate eFile and set the pointer to NULL.
 *
 * @param efile eFile pointer pointer
 */
void delete_eFile(eFile **efile);


/**
 * @brief the add_empty_line_eFile() add an empty line to the position pos in the file
 *
 * @param efile eFile pointer pointer
 * @param pos  Position of the new line in the file
 */
int add_empty_line_eFile(eFile *efile, unsigned int pos);


/**
 * @brief the delete_line_eFile() delete a line to the position pos in the file
 *
 * @param efile eFile pointer pointer
 * @param pos Position of the line that will be deleted
 */
void delete_line_eFile(eFile *efile, unsigned int pos);


/**
 * @brief the insert_char_eFile() insert a character to the current position in the current_line 
 *
 * @param efile eFile pointer
 * @param ch character to insert
 */
void insert_char_eFile(eFile *efile, const char ch);


/**
 * @brief the remove_char_eFile() remove a character to the current position in the current_line 
 *
 * @param efile eFile pointer 
 */
void remove_char_eFile(eFile *efile);


/**
 * @brief the insert_string_eFile() insert the string at current_pos in the current line 
 *
 * @param efile eFile pointer
 * @param string string to insert
 * @param length lenght of string
 */
void insert_string_eFile(eFile *efile, const char *string, size_t length);


/**
 * @brief the remove_string_eFile() remove the string that start at current_pos and end to current_pos+length in the current line 
 *
 * @param efile eFile pointer
 * @param length of string to remove
 */
void remove_string_eFile(eFile *efile, size_t length);

#endif
