/**
 * @file 		eBar.h
 * @brief 		eBar Header
 * @author 		ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright 	GNU Public License.
 *
 */


#ifndef __EBAR_H__
#define __EBAR_H__

#include "eFile.h"


/**
 * @struct eBar structure to store opened files.
 */
typedef struct {

	/** List allocation size */
	size_t alloc_size;

	/** List of eFile */
	eFile const **files;

	/** File count */
	unsigned int n_files;
	
} eBar;


/**
 * @brief The create_eBar() function allocate and initialize an eBar.
 *
 * @return Pointer on the eBar structure or NULL if allocation failed.
 *
 * @note delete_eBar() must be called before exiting.
 */
eBar * create_eBar(void);


/**
 * @brief The delete_eBar() function delete and deallocate eBar and set pointer to NULL.
 *
 * @param bar: eBar pointer pointer
 */
void delete_eBar(eBar ** bar);


/**
 * @brief The add_file_eBar() function add a file to the bar.
 *
 * @param bar: eBar pointer
 * @param file: eFile pointer
 *
 * @return 0 on success or -1 in failure.
 */
int add_file_eBar(eBar * bar, eFile const * file);


/**
 * @brief The remove_file_eBar() function remove a file from the bar.
 *
 * @param bar: eBar pointer
 * @param index: index of eFile to remove
 *
 * @return 0 on success or -1 in failure.
 */
int remove_file_eBar(eBar * bar, unsigned int index);


/**
 * @brief The get_file_eBar() function return the eFile pointer at the specified index.
 *
 * @param bar: eBar pointer
 * @param index: index of eFile to remove 
 *
 * @return eFile pointer on sucess or NULL in failure.
 */
eFile const * get_file_eBar(eBar const * bar, unsigned int index);


/**
 * @brief The count_eBar() function return the number of file stored in eBar.
 *
 * @param bar: eBar pointer
 *
 * @return number of eFile stored in eBar.
 */
unsigned int count_eBar(eBar const * bar);


/**
 * @brief The is_file_in_bar() function return true if the file is in the bar.
 *
 * @param bar: eBar pointer
 * @param file: eFile pointer
 *
 * @return true if the file is in the bar, false otherwise.
 */
bool is_file_in_eBar(eBar const * bar, eFile const * file);

#endif
