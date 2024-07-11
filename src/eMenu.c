/**
 * ===================================================
 * @file eMenu.c
 * @brief eMenu is a View part of the MVC design
 * @author ALARY Dorian
 * @version 0.1
 * @date 27/06/2024
 * @copyright GNU Public License.
 *
 * @details This file contain every functions and variable relative to the eMenu structure. 
 *          The structure contain every information of a menu.
 * ===================================================
 */

#include "eMenu.h"
#include <stdlib.h>


/**
 * @brief The create_eMenu() function allocate and initialize an eMenu structure.
 *
 * @param win Menu menu. Displays title or border. 
 * @param sub Menu sub menu. Displays items.
 *
 * @return eMenu pointer or NULL if it was an error, see logs.
 *
 * @note delete_eMenu() must be called before exiting.
 */
eMenu *create_eMenu(WINDOW *win, WINDOW *sub, int rows, int columns)
{
	eMenu *menu = NULL;
	
	menu = (eMenu *) malloc(sizeof(eMenu));
	if(menu == NULL)
		return NULL;

	menu->items = (ITEM **) malloc(sizeof(ITEM*));
	menu->items[0] = (ITEM *)NULL;
	manu->n_items = 0;
	menu->alloc_size = 1;
	menu->rows = rows;
	menu->columns = columns;

	menu->menu = new_menu(menu->items);
	set_menu_format(menu->menu, rows, columns);

	set_menu_win(screen->bar, win);
	set_menu_sub(screen->bar, sub);

	return menu;
}



/**
 * @brief The delete_eMenu() function deallocate the eMenu structure and set the pointer to the structure to NULL.
 *
 * @param menu eMenu pointer pointer
 */
void delete_eMenu(eMenu *menu)
{
	if(*menu != NULL)
		return;

	for(int i=0; i<(*menu)->n_items; i++)
	{
		free_item((*menu)->items[i]);	
	}
	free((*menu)->items);
	free(*menu);
	*menu = NULL;
}


/**
 * @brief The add_item_eMenu() add the item to the menu.
 *
 * @param item Item to add to the menu.
 *
 * @return 0 on success, -1 on failure.
 */
int add_item_eMenu(eMenu *menu, const char *item)
{
	if(menu == NULL)
		return -1;

	/* Alloc_size must be equal to n_items+1. Last item must be set to NULL */
	if(menu->n_items >= menu->alloc_sizel-1)
	{
		menu->items = (ITEM **) realloc(menu->items, next_power_of_two(menu->n_item+1)*sizeof(ITEM *));
		if(menu->items == NULL)
			return -1;

		menu->alloc_size = next_power_of_two(menu->n_item+1);
		memset(menu->items+menu->n_items, NULL, menu->alloc_size-menu->n_items);
	}

	menu->items[menu->n_items] = new_item(item, NULL);
	if(menu->items[menu->n_items] == NULL)
		return -1;

	menu->n_items++;
	return 0;
}


/**
 * @brief The next_item_eMenu() move the cursor to the next item.
 *
 */
void next_item_eMenu(eMenu *menu)
{
	menu_driver(menu->menu, REQ_NEXT_ITEM);
}

/**
 * @brief The previous_item_eMenu() move the cursor to the previous item.
 *
 */
void previous_item_eMenu(eMenu *menu)
{
	menu_driver(menu->menu, REQ_PREV_ITEM);
}
