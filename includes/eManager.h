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
#include "eBar.h"
#include "eDirectory.h"


/**
 * @enum Program mode enumeration
 */
typedef enum {
	DIR,
	WRITE,
	BAR

} MODE;


/**
 * @struct eManager structure to play the Controler role in MVC 
 */
typedef struct {

	/* Editor mode */
	MODE mode;

	/* Screen */
	eScreen *screen;

	/* Current opened file */
	eFile *file;

	/* File bar */
	eBar *bar;

	/* Directory */
	eDirectory *directory;

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
 * return 0 on success, -1 on error
 * @note This function also print the content of the file in the screen.
 */
int set_eFile_eManager(eManager *manager, eFile *file);


/**
 * @brief The set_eBar_eManager() function set an eBar to eManager.
 *
 * @param manager eManager pointer
 * @param bar eBar pointer
 *
 */
void set_eBar_eManager(eManager *manager, eBar *bar);


/**
 * @brief The set_eDirectory_eManager() function set an eDirectory to eManager.
 *
 * @param manager eManager pointer
 * @param directory eDirectory pointer
 *
 * @note This function also print the content of the repository in the screen.
 */
void set_eDirectory_eManager(eManager *manager, eDirectory *directory);


/**
 * @brief The run_eManager() function is the main function of eManager, this function call screen to get an input and process the input.
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool run_eManager(eManager *manager);


/*
 * @brief The getx_cursor_eManager() return the window file x position of cursor in fuction of file.
 *
 * @param manager eManager pointer
 *
 * @return returns x position.
 */
unsigned int getx_cursor_eManager(eManager *manager);


/*
 * @brief The gety_cursor_eManager() return the window file y position of cursor in fuction of file.
 *
 * @param manager eManager pointer
 *
 * @return returns y position.
 */
unsigned int gety_cursor_eManager(eManager *manager);


int fill_directory_menu_eManager(eManager *manager, eDirectory *directory, unsigned int level);
#endif
