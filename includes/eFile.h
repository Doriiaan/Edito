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


/**
 * @enum File permissions enumeration
 */
typedef enum
{
	p_NOPERM,
	p_READONLY,
	p_READWRITE,
	p_CREATE
} PERM;


/**
 * @struct eFile structure to reprensent a file in memory 
 */
typedef struct efile_s
{
	/** File permissions */
	PERM permissions;

	/** Number of line */
	unsigned int n_elines;
	
	/** First line of eLine linked list */
	eLine *first;

	/** Filename */
	const char *filename;

} eFile;


/**
 * @brief The create_eFile() function allocate and initialize an eFile structure.
 *
 * @param filename:	Name of the file
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile* create_eFile(const char *filename);


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

#endif
