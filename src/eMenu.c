/**
 * ===================================================
 * @file eMenu.c
 * @brief eMenu is a View part of the MVC design
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
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

static void init_menu(eMenu * menu);
static void reset_menu(eMenu * menu);


/**
 * @brief The create_eMenu() function allocate and initialize an eMenu structure.
 *
 * @param win: Menu window
 * @param sub: Menu sub window
 * @param columnar: true, if the menu displays items in columns, false if it displays them in rows
 *
 * @return eMenu pointer or NULL if it was an error.
 *
 * @note delete_eMenu() must be called before exiting.
 */
eMenu * create_eMenu(WINDOW * win, WINDOW * sub, bool columnar)
{
	eMenu *menu = NULL;

	menu = (eMenu *) malloc(sizeof(eMenu));
	if(menu == NULL)
		return NULL;

	menu->win = win;
	menu->sub = sub;
	menu->virtual_items_title = (char **) malloc(sizeof(char*));
	menu->virtual_items_title[0] = NULL;
	menu->alloc_size = 1;
	menu->n_items = 0;

	menu->menu = NULL;
	menu->items = NULL;
	menu->physical_items_title = NULL;
	menu->rows = 1;
	menu->columns = 1;
	menu->columnar = columnar;
	menu->n_scroll = 0;

	return menu;
}


/**
 * @brief The delete_eMenu() function deallocate the eMenu structure and set the pointer to the structure to NULL.
 *
 * @param menu: eMenu pointer pointer
 */
void delete_eMenu(eMenu ** menu)
{
	if(*menu != NULL)
		return;

	/* Delete physical menu */
	reset_menu(*menu);

	for(int i=0; i<(*menu)->n_items; i++)
	{
		free((*menu)->virtual_items_title[i]);
	}

	if((*menu)->virtual_items_title != NULL)
		free((*menu)->virtual_items_title);

	free(*menu);
	*menu = NULL;
}


/**
 * @brief The add_item_eMenu() add the item to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 * @param item: Item added
 *
 * @return 0 on success, -1 in failure.
 */
int add_item_eMenu(eMenu * menu, char const * item)
{
	int sub_cols=0, sub_rows=0;

	if(menu == NULL)
		return -1;

	/* Alloc_size must be equal to n_items+1. */
	if(menu->n_items+1 > menu->alloc_size)
	{
		menu->virtual_items_title = (char **) realloc(menu->virtual_items_title, get_next_power_of_two(menu->n_items+1)*sizeof(char *));
		if(menu->virtual_items_title == NULL)
			return -1;

		menu->alloc_size = get_next_power_of_two(menu->n_items+1);
	}

	menu->virtual_items_title[menu->n_items] = strdup(item);
	if(menu->virtual_items_title[menu->n_items] == NULL)
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
 * @brief The delete_item_eMenu() delete the item to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 * @param item: Item deleted
 *
 * @return 0 on success, -1 in failure.
 */
int delete_item_eMenu(eMenu * menu, int index)
{
	if(menu == NULL || index >= menu->n_items)
		return -1;

	free(menu->virtual_items_title[index]);
	menu->n_items--;

	for(int i=index; i<menu->n_items; i++)
	{
		menu->virtual_items_title[i] = menu->virtual_items_title[i+1];
	}
	menu->virtual_items_title[menu->n_items] = NULL;

	return 0;
}


/**
 * @brief The erase_eMenu() erase the virtual menu. Useful to rebuild virtual menu.
 *
 * @param menu: eMenu pointer pointer
 */
void erase_eMenu(eMenu * menu)
{
	if(menu==NULL)
		return;

	if(menu->virtual_items_title==NULL)
		return;

	for(int i=0; i<menu->n_items; i++)
	{
		if(menu->virtual_items_title[i] != NULL)
		{
			free(menu->virtual_items_title[i]);
			menu->virtual_items_title[i] = NULL;
		}
	}

	menu->n_items = 0;
}


/**
 * @brief The refresh_eMenu() set the physical menu to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 */
void refresh_eMenu(eMenu * menu)
{
	int pos = get_current_item_index_eMenu(menu);
	unpost_menu(menu->menu);
	reset_menu(menu);
	init_menu(menu);
	post_menu(menu->menu);
	set_cursor_position_eMenu(menu, pos);
}


/**
 * @brief The move_next_item_eMenu() move the cursor to the next item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_next_item_eMenu(eMenu * menu)
{
	int max_height = 0;
	int max_width = 0;

	int cur_height = 0;
	int cur_width = 0;

	getmaxyx(menu->sub, max_height, max_width);
	getyx(menu->sub, cur_height, cur_width);
	(void)max_width;
	(void)cur_width;

	/* calculates the number of scrolls */
	if(!menu->columnar && max_height-1 == cur_height && get_current_item_index_eMenu(menu) != item_count(menu->menu)-1)
	{
		menu->n_scroll++;
	}

	menu_driver(menu->menu, REQ_NEXT_ITEM);
}


/**
 * @brief The move_previous_item_eMenu() move the cursor to the previous item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_previous_item_eMenu(eMenu * menu)
{
	int cur_height = 0;
	int cur_width = 0;

	getyx(menu->sub, cur_height, cur_width);
	(void)cur_width;

	/* calculates the number of scrolls */
	if(!menu->columnar && cur_height == 0 && get_current_item_index_eMenu(menu) != 0)
	{
		menu->n_scroll--;
	}

	menu_driver(menu->menu, REQ_PREV_ITEM);
}


/**
 * @brief The move_current_item_eMenu(eMenu *menu) move the cursor to the current item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_current_item_eMenu(eMenu * menu)
{
	pos_menu_cursor(menu->menu);
}


/**
 * @brief The move_pattern_item_eMenu() function move the cursor to the next item with pattern matching.
 *
 * @param menu: eMenu pointer pointer
 * @param pattern: pattern to match
 */
void move_pattern_item_eMenu(eMenu * menu, char const * pattern)
{
	set_menu_pattern(menu->menu, pattern);
}


/**
 * @bried The get_current_item_index_eMenu() function return the current item index.
 *
 * @param menu: eMenu pointer pointer
 *
 * @return The current item index.
 */
int get_current_item_index_eMenu(eMenu const * menu)
{
	return item_index(current_item(menu->menu));
}


/**
 * @brief The set_cursor_position_eMenu() set the menu current position to the specified position.
 *
 * @param menu: eMenu pointer pointer
 * @param position: New current position
 */
void set_cursor_position_eMenu(eMenu * menu, int position)
{
	for(int i=0; i<menu->n_scroll; i++)
	{
		menu_driver(menu->menu, REQ_SCR_DLINE);
	}

	for(int i=0; i < position-menu->n_scroll; i++)
	{
		menu_driver(menu->menu, REQ_DOWN_ITEM);
	}
}


/**
 * @brief the reset_menu() function, free the ncurses items and menu.
 *
 * @param menu: eMenu pointer pointer
 */
void reset_menu(eMenu * menu)
{
	int i=0;
	int count = item_count(menu->menu);

	if(menu->menu != NULL)
		free_menu(menu->menu);
	menu->menu = NULL;

	for(i=0; i<count; i++)
	{
		if(menu->items[i] != NULL)
		{
			free_item(menu->items[i]);
			menu->items[i]=NULL;
		}
		if(menu->physical_items_title[i] != NULL)
		{
			free(menu->physical_items_title[i]);
			menu->physical_items_title[i] = NULL;
		}
	}

	if(menu->items != NULL)
	{
		free(menu->items);
		menu->items = NULL;
	}

	if(menu->physical_items_title != NULL)
	{
		free(menu->physical_items_title);
		menu->physical_items_title = NULL;
	}
}


/**
 * @brief the init_menu() function, create the ncurses items and menu with menu->virtual_items_title.
 *
 * @param menu: eMenu pointer pointer
 */
void init_menu(eMenu * menu)
{
	int i=0;
	menu->physical_items_title = (char **) malloc(sizeof(char *)*menu->n_items);
	if(menu->physical_items_title == NULL)
		return;

	menu->items = (ITEM **) malloc(sizeof(ITEM *)*(menu->n_items+1));
	if(menu->items == NULL)
		return;

	for(i=0; i<menu->n_items; i++)
	{
		menu->physical_items_title[i] = strdup(menu->virtual_items_title[i]);
		menu->items[i] = new_item(menu->physical_items_title[i], "");
		item_opts_off(menu->items[i], O_NONCYCLIC | O_SHOWDESC);
	}
	menu->items[menu->n_items] = NULL;

	menu->menu = new_menu(menu->items);
	set_menu_format(menu->menu, menu->rows, menu->columns);
	set_menu_mark(menu->menu, "");

	/* description 1 blank, rows default line, columns 1 blank */
	set_menu_spacing(menu->menu, 1, 0, 1);

	set_menu_win(menu->menu, menu->win);
	set_menu_sub(menu->menu, menu->sub);
}
