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
#include "util.h"
#include <stdlib.h>
#include <string.h>

static void init_menu(eMenu *menu);
static void reset_menu(eMenu *menu);

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
eMenu *create_eMenu(WINDOW *win, WINDOW *sub, int rows, int columns, bool columnar)
{
	eMenu *menu = NULL;
	
	menu = (eMenu *) malloc(sizeof(eMenu));
	if(menu == NULL)
		return NULL;

	menu->win = win;
	menu->sub = sub;
	menu->items_title = (char **) malloc(sizeof(char*));
	menu->items_title[0] = NULL;
	menu->items = (ITEM **) malloc(sizeof(ITEM*));
	menu->items[0] = (ITEM *)NULL;
	menu->n_items = 0;
	menu->alloc_size = 1;
	menu->rows = rows;
	menu->columns = columns;
	menu->columnar = columnar;

	menu->menu = new_menu(menu->items);
	set_menu_format(menu->menu, menu->rows, menu->columns);
	set_menu_mark(menu->menu, "");

	set_menu_win(menu->menu, menu->win);
	set_menu_sub(menu->menu, menu->sub);

	return menu;
}



/**
 * @brief The delete_eMenu() function deallocate the eMenu structure and set the pointer to the structure to NULL.
 *
 * @param menu eMenu pointer pointer
 */
void delete_eMenu(eMenu **menu)
{
	if(*menu != NULL)
		return;

	for(unsigned int i=0; i<(*menu)->n_items; i++)
	{
		free_item((*menu)->items[i]);	
	}
	free((*menu)->items);

	for(unsigned int i=0; i<(*menu)->n_items; i++)
	{
		free((*menu)->items_title[i]);	
	}

	free((*menu)->items_title);
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
	int sub_cols=0, sub_rows=0;

	if(menu == NULL)
		return -1;
	
	/* Alloc_size must be equal to n_items+1. Last item must be set to NULL */
	if(menu->n_items >= menu->alloc_size)
	{
		menu->items_title = (char **) realloc(menu->items_title, get_next_power_of_two(menu->n_items)*sizeof(char *));
		if(menu->items_title == NULL)
			return -1;

		menu->alloc_size = get_next_power_of_two(menu->n_items);
	}

	menu->items_title[menu->n_items] = strdup(item);
	if(menu->items_title[menu->n_items] == NULL)
		return -1;

	menu->n_items++;

	getmaxyx(menu->sub, sub_rows, sub_cols);
	if(menu->columnar && menu->columns < sub_cols)
		menu->columns++;
	else if(!menu->columnar && menu->rows < sub_rows)
		menu->rows++;

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

/**
 * @brief The refresh_eMenu() function refresh the menu on the screen
 *
 */
void refresh_eMenu(eMenu *menu)
{
	reset_menu(menu);
	init_menu(menu);
	post_menu(menu->menu);
}

void reset_menu(eMenu *menu)
{
	int i=0;
	int count = item_count(menu->menu); 

	free_menu(menu->menu);
	menu->menu = NULL;

	for(i=0; i<count; i++)
	{
		if(menu->items[i] != NULL)
		{
			free_item(menu->items[i]);
			menu->items[i]=NULL;
		}
	}
	if(menu->items != NULL)
	{
		free(menu->items);
		menu->items = NULL;
	}
}

void init_menu(eMenu *menu)
{
	unsigned int i=0;
	menu->items = (ITEM **) malloc(sizeof(ITEM *)*(menu->n_items+1));

	for(i=0; i<menu->n_items; i++)
	{
		menu->items[i] = new_item(menu->items_title[i], "");
		item_opts_off(menu->items[i], O_NONCYCLIC | O_SHOWDESC);
	}
	menu->items[menu->n_items] = NULL;
	
	menu->menu = new_menu(menu->items);
	set_menu_format(menu->menu, menu->rows, menu->columns);
	set_menu_mark(menu->menu, "");

	set_menu_win(menu->menu, menu->win);
	set_menu_sub(menu->menu, menu->sub);
}
