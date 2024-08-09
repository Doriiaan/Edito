/**
 * @file eManager.h
 * @brief eManager Header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
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

	/** Screen */
	eScreen * screen;

	/** Current opened file */
	eFile * file;

	/** File bar */
	eBar * bar;

	/** Directory */
	eDirectory * directory;

	/** Current mode */
	MODE mode;

	/** Last mode */
	MODE lastmode;

	/** Help message */
	char * help_msg;

} eManager;


/**
 * @brief The create_eManager() function allocate and initialize an
 *        eManager structure.
 *
 * @return eManager pointer or NULL if it was an error, see logs.
 *
 * @note delete_eManager() must be called before exiting.
 */
eManager * create_eManager(void);


/**
 * @brief The delete_eManager() function deallocate and set the pointer
 *        to eManager structure to NULL.
 *
 * @param manager: eManager pointer pointer
 */
void delete_eManager(eManager ** manager);


/**
 * @brief The set_eScreen_eManager() function set a screen to eManager.
 *
 * @param manager: eManager pointer
 * @param screen: eScreen pointer
 */
void set_eScreen_eManager(eManager * manager,
		                  eScreen * screen);


/**
 * @brief The set_eBar_eManager() function set an eBar to eManager.
 *
 * @param manager: eManager pointer
 * @param bar: eBar pointer
 */
void set_eBar_eManager(eManager * manager,
		               eBar * bar);


/**
 * @brief The set_eDirectory_eManager() function set an eDirectory to
 *        eManager.
 *
 * @param manager: eManager pointer
 * @param directory: eDirectory pointer
 */
void set_eDirectory_eManager(eManager * manager,
		                     eDirectory * directory);


/**
 * @brief The set_eFile_eManager() function set an eFile to eManager.
 *
 * @param manager: eManager pointer
 * @param file: eFile pointer
 */
void set_eFile_eManager(eManager * manager,
		                eFile * file);


/**
 * @brief The run_eManager() function is the main function of eManager,
 *        this function call screen to get an input and process the input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool run_eManager(eManager * manager);


/*
 * @brief The getx_cursor_eManager() return the position x of the cursor
 *        in the file window depending on the current file.
 *
 * @param manager: eManager pointer
 *
 * @return returns x position.
 */
unsigned int getx_cursor_eManager(eManager const * manager);


/*
 * @brief The gety_cursor_eManager() return the position y of the cursor
 *        in the file window depending on the current file.
 *
 * @param manager: eManager pointer
 *
 * @return returns y position.
 */
unsigned int gety_cursor_eManager(eManager const * manager);


/**
 * @brief The fill_directory_menu_eManager() function fill the directory
 *        screen menu depending on the manager directory.
 *
 * @param manager: eManager pointer
 * @param directory: pointer on the root directory (for recursivity)
 * @param level: level of the directory. The first must be 0
 *
 * @return 0 on success or -1 in failure.
 * @note This is a recursive function.
 */
int fill_directory_menu_eManager(eManager const * manager,
		                         eDirectory const * directory,
								 unsigned int level);


/**
 * @brief The print_file_eManager() functin print the content of the
 *        current file on the screen.
 *
 * @param manager: eManager pointer
 *
 * return 0 on success or -1 in failure.
 */
int print_file_eManager(eManager const * manager);


/**
 * @brief The send_help_msg_to_screen_eManager() function send help
 *        message to the screen.
 *
 * @note If no message is set, send default help message.
 * @note If a message is set, sending it will delete it
 *
 * @param manager: eManager pointer
 */
void send_help_msg_to_screen_eManager(eManager * manager);

#endif
