/**
 * @file 		eMenu.h
 * @brief 		eMenu Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __EMENU_H__
#define __EMENU_H__

#include <ncurses.h>
#include <menu.h>

#define MENU_NUMBER 2


/**
 * @enum Menu type enumeration
 */
typedef enum {

	MDIR=0,
	MBAR

} MENU_TYPE;


/**
 * @struct Menu structure to keep all information about a menu
 */
typedef struct {

	size_t alloc_size;

	WINDOW *win;

	WINDOW *sub;

	MENU *menu;
	
	ITEM **items;
	
	char **virtual_items_title;

	char **physical_items_title;

	int rows;

	int columns;

	unsigned int n_items;

	bool columnar; /* 1 in column, 0 in row */ 

} eMenu;


/**
 * @brief The create_eMenu() function allocate and initialize an eMenu structure.
 *
 * @param win Menu menu. Displays title or border. 
 * @param sub Menu sub menu. Displays items.
 * @param columnar, true, if the table displays elements in columns, false if it displays them in rows
 *
 * @return eMenu pointer or NULL if it was an error, see logs.
 *
 * @note delete_eMenu() must be called before exiting.
 */
eMenu *create_eMenu(WINDOW *win, WINDOW *sub, bool columnar);


/**
 * @brief The delete_eMenu() function deallocate the eMenu structure and set the pointer to the structure to NULL.
 *
 * @param menu eMenu pointer pointer
 */
void delete_eMenu(eMenu **menu);


/**
 * @brief The add_item_eMenu() add the item to the menu.
 *
 * @param item Item to add to the menu.
 *
 * @return 0 on success, -1 on failure.
 */
int add_item_eMenu(eMenu *menu, const char *item);


/**
 * @brief The next_item_eMenu() move the cursor to the next item.
 *
 */
void next_item_eMenu(eMenu *menu);

/**
 * @brief The previous_item_eMenu() move the cursor to the previous item.
 *
 */
void previous_item_eMenu(eMenu *menu);

void refresh_eMenu(eMenu *menu);
#endif
