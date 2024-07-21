/**
 * ===================================================
 * @file eBar.c
 * @brief Contain eBar structure and functions
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 * @details This file contains all the structures, variables and functions used to manage the bar structure. 
 * 			eBar is used to store open efile. This structure do not manage any screen function.
 * ===================================================
 */

#include "eBar.h"
#include "eFile.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>


/**
 * @brief The create_eBar() function allocate and initialize an eBar.
 *
 * @return Pointer on the eBar structure or NULL if allocation failed.
 *
 * @note delete_eBar() must be called before exiting.
 */
eBar *create_eBar()
{
	eBar *bar = NULL;

	bar = (eBar *) malloc(sizeof(eBar));
	
	bar->files = NULL;
	bar->n_files = 0;
	bar->alloc_size = 0;

	return bar;
}


/**
 * @brief The delete_eBar() function delete and deallocate eBar and set pointer to NULL.
 *
 * @param bar: eBar pointer pointer
 */
void delete_eBar(eBar **bar)
{
	if(*bar == NULL)
		return;

	if((*bar)->files)
		free((*bar)->files);

	free(*bar);
	*bar = NULL;
}


/**
 * @brief The add_file_eBar() function add a file to the bar.
 *
 * @param bar: eBar pointer
 * @param file: eFile pointer
 *
 * @return 0 on success or -1 in failure.
 */
int add_file_eBar(eBar *bar, eFile *file)
{
	if(bar == NULL || file == NULL)
		return -1;

	/* If the list isn't large enough */
	if(bar->n_files >= bar->alloc_size)
	{
		bar->alloc_size = get_next_power_of_two(bar->n_files);
		bar->files = (eFile **) realloc(bar->files, bar->alloc_size*sizeof(eFile));
		if(bar->files == NULL)
		{
			return -1;
		}
	}

	bar->files[bar->n_files] = file;
	bar->n_files++;

	return 0;
}


/**
 * @brief The remove_file_eBar() function remove a file from the bar.
 *
 * @param bar: eBar pointer
 * @param index: index of eFile to remove
 *
 * @return 0 on success or -1 in failure.
 */
int remove_file_eBar(eBar *bar, unsigned int index)
{
	if(bar == NULL || index >= bar->n_files)
		return -1;

	//TODO: close eFile
	bar->files[index] = NULL;
	bar->n_files--;
	return 0;
}


/**
 * @brief The get_file_eBar() function return the eFile pointer at the specified index.
 *
 * @param bar: eBar pointer
 * @param index: index of eFile to remove 
 *
 * @return eFile pointer on sucess or NULL in failure.
 */
eFile *get_file_eBar(eBar *bar, unsigned int index)
{
	if(bar == NULL || index >= bar->n_files)
		return NULL;

	return bar->files[index];
}


/**
 * @brief The count_eBar() function return the number of file stored in eBar.
 *
 * @param bar: eBar pointer
 *
 * @return number of eFile stored in eBar.
 */
unsigned int count_eBar(eBar *bar)
{
	return bar->n_files;
}


/**
 * @brief The is_file_in_bar() function return true if the file is in the bar.
 *
 * @param bar: eBar pointer
 * @param file: eFile pointer
 *
 * @return true if the file is in the bar, false otherwise.
 */
bool is_file_in_eBar(eBar *bar, eFile *file)
{
	if(bar == NULL ||file == NULL)
		return false;

	for(unsigned int i; i<bar->n_files; i++)
	{
		if(!strcmp(bar->files[i]->filename, file->filename))
			return true;
	}
	return false;
}
