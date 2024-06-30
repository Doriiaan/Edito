/**
 * @file 		eManager.h
 * @brief 		eManager Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __EMANAGER_H__
#define __EMANAGER_H__

#include "eFile.h"
#include "eScreen.h"
#include "eLine.h"


/**
 * @enum Program mode enumeration
 */
typedef enum {
	NORMAL,
	WRITE	

} MODE;


/**
 * @struct eManager structure to play the Controler role in MVC 
 */
typedef struct {

	/* Editor mode */
	MODE mode;

	eScreen *screen;

	eFile *file;

	eLine *current_line;

	eLine *first_screen_line;

} eManager;


/**
 * @brief The create_eManager() function allocate and initialize an eManager structure.
 *
 * @return eManager pointer or NULL if it was an error, see logs.
 *
 * @note delete_eManager() must be called before exiting.
 */

eManager *create_eManager();


/**
 * @brief The delete_eManager() function deallocate and set the pointer to eManager structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eManager(eManager **manager);


/**
 * @brief The set_eScreen_eManager() function set a screen to eManager.
 *
 * @param manager eManager pointer
 * @param screen eScreen pointer
 */
void set_eScreen_eManager(eManager *manager, eScreen *screen);


/**
 * @brief The set_eFile_eManager() function set an eFile to eManager.
 *
 * @param manager eManager pointer
 * @param file eFile pointer
 *
 * @note This function also print the content of the file in the screen.
 */
void set_eFile_eManager(eManager *manager, eFile *file);


/**
 * @brief The run_eManager() function is the main function of eManager, this function call screen to get an input and process the input.
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool run_eManager(eManager *manager);

#endif
