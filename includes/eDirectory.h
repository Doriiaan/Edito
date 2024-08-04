/**
 * @file 		eDirectory.h
 * @brief 		eDirectory Header
 * @author 		ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __EDIRECTORY_H__
#define __EDIRECTORY_H__

#include "eFile.h"
#include "util.h"

#include <stdbool.h>


/**
 * @struct eDirectory structure to store a directory and files structure. Part of N-ary tree.
 */
typedef struct eDirectory {

	/** Child files allocation memory*/
	size_t alloc_files_size;
	
	/** Child directories allocation memory */
	size_t alloc_dirs_size;
	
	/** List of child files */
	eFile **files;

	/** List of child directories */
	struct eDirectory **dirs;

	/** Number of child files */
	unsigned int n_files;
	
	/** Number of child directories */
	unsigned int n_dirs;

	/** Directory permissions */
	PERM permissions;
	
	/** Directory name */
	char *dirname;

	/** Directory path + '/' + name */
	char *realpath;

	/** Do the directory is open (on screen) */
	bool is_open;

} eDirectory;


/**
 * @brief The create_eDirectory() function allocate and initialize an eDirectory and is child directories andd files.
 *
 * @param realpath: Path + '/' + name of the directory
 *
 * @return Pointer on the eDirectory structure or NULL if allocation failed.
 *
 * @note delete_eDirectory() must be called before exiting.
 */
eDirectory * create_eDirectory(char const * realpath);


/**
 * @brief The delete_eDirectory() function delete and deallocate eDirectory and set pointer to NULL.
 *
 * @param directory: eDirectory pointer pointer
 */
void delete_eDirectory(eDirectory ** directory);


/** 
 * @brief The get_item_at_index_eDirectory() function returns either a directory or a file.
 *
 * @param directory: eDirectory pointer
 * @param item_index: item of pointer to be returned
 * @param out_directory: eDirectory pointer returned
 * @param out_file: eFile pointer returned
 *
 * @return 0 on success or -1 in failure or a positive number indicating the index overflow in comparison with the number of files/folders.
 */
int get_item_at_index_eDirectory(eDirectory const * directory, unsigned int item_index, eDirectory ** out_directory, eFile ** out_file);

#endif
