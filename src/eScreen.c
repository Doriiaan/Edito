/**
 * ===================================================
 * @file eScreen.c
 * @brief Contain eScreen structure and functions
 * @author ALARY Dorian
 * @version 0.1
 * @date 27/06/2024
 * @copyright GNU Public License.
 *
 * @details This file contains all the structures, variables and functions used to manage the screen and its windows. 
 *          The methods allow you to :
 *              - initialize and allocate eScreen from the width and height of the screen,
 *              - manage content of the screen.
 * ===================================================
 */

#include "eScreen.h"
#include "eLine.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>



/**
 * @brief The create_eScreen() function allocate and initialize an eScreen structure.
 *
 * @return eScreen pointer or NULL if it was an error, see logs.
 *
 * @note delete_eScreen() must be called before exiting.
 */
eScreen *create_eScreen(int lines, int columns)
{
	eScreen *screen = NULL;
	int width_repository = 0, height_repository = 0;
	int width_bar = 0, height_bar = 0;
	int width_file_box = 0, height_file_box = 0;

	screen = (eScreen *) malloc(sizeof(eScreen));
	if(screen == NULL)
	{
		return NULL;
	}

	/* Set eScreen dimension */
	screen->width = columns;
	screen->height = lines;
	
	/* Set WINDOWs dimension */
	width_repository = columns/5;
	height_repository = lines;

	width_bar = columns - width_repository;
	height_bar = lines/10;

	width_file_box = width_bar;
	height_file_box = lines - height_bar;
	

	/* Create WINDOWs */
	screen->windows[REPOSITORY] = create_eWindow(height_repository, width_repository, 0, 0);

	screen->windows[BAR_BOX] = create_eWindow(height_bar, width_bar,0 , width_repository);
	
	screen->windows[BAR_ITEMS] = create_der_eWindow(screen->windows[BAR_BOX], height_bar-2, width_bar-2 , 1 , 1);

	screen->windows[FILE_BOX] = create_eWindow(height_file_box, width_file_box, height_bar, width_repository);

	screen->windows[FILE_LINESNUMBER] = NULL; 

	screen->windows[FILE_CONTENT] = NULL;

	screen->windows[POPUP] = NULL;

	screen->current_window = screen->windows[REPOSITORY];

	/* Create MENUs */
	screen->menus[BAR] = create_eMenu(screen->windows[BAR_BOX], screen->windows[BAR_ITEMS]);

	// screen->menus[REPOSITORY] = create_eMenu(screen->windows[REPOSITORY], screen->windows[REPOSITORY_ITEMS]);

	return screen;
}


/**
 * @brief The delete_eScreen() function deallocate the eScreen structure and set the pointer to the structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eScreen(eScreen **screen)
{
	if(*screen == NULL)
		return;

	for(int i=0; i<WINDOWS_NUMBER; i++)
	{
		if((*screen)->windows[i] != NULL)
			delete_eWindow(&(*screen)->windows[i]);
	}
	free(*screen);
	*screen = NULL;
}


/**
 * @brief The update_repository_eScreen() function refresh the repository window.
 *
 * @param screen eScreen pointer
 */
void update_repository_eScreen(eScreen *screen)
{
	box(screen->windows[REPOSITORY]->window, 0, 0);
	wnoutrefresh(screen->windows[REPOSITORY]->window);
	doupdate();
}


/*
 * @brief The update_bar_eScreen() function refresh the bar window.
 *
 * @param screen eScreen pointer
 */
void update_bar_eScreen(eScreen *screen)
{
	box(screen->windows[BAR_BOX]->window, 0, 0);
	post_menu(screen->bar);
	wnoutrefresh(screen->windows[BAR_BOX]->window);
	doupdate();
}


/**
 * @brief The update_file_eScreen() function refresh the file content and file number window.
 *
 * @param screen eScreen pointer
 */
void update_file_eScreen(eScreen *screen)
{
	wnoutrefresh(screen->windows[FILE_LINESNUMBER]->window);
	wnoutrefresh(screen->windows[FILE_CONTENT]->window);
	doupdate();
}


/**
 * @brief the update_all_eScreen() function refresh all the window of the screen.
 *
 * @param screen eScreen pointer
 */
void update_all_eScreen(eScreen *screen)
{
	box(screen->windows[REPOSITORY]->window, 0, 0);
	box(screen->windows[BAR_BOX]->window, 0, 0);
	box(screen->windows[FILE_BOX]->window, 0, 0);

	post_menu(screen->bar);
	
	for(int i=0 ; i<WINDOWS_NUMBER ; i++)
	{
		if(screen->windows[i])
			wnoutrefresh(screen->windows[i]->window);
	}
	
	doupdate();
}


/**
 * @brief The set_current_eScreen() function set the current window and cursor of the screen.
 *
 * @param type The type of the window (the type refers to the window)
 */
void set_current_window_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	screen->current_window = screen->windows[type];
}


/**
 * @brief The get_input_eScreen() function request an input to the user.
 *
 * @param screen eScreen pointer
 */
int get_input_eScreen(eScreen *screen)
{
	return wgetch(screen->current_window->window);
}


void move_cursor_eScreen(eScreen *screen, WINDOW_TYPE type, unsigned int y, unsigned int x)
{
	wmove(screen->windows[type]->window, y, x);
}


unsigned int get_width_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	return screen->windows[type]->width;
}


unsigned int get_height_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	return screen->windows[type]->height;
}



/* ==========================================================
 * eWindow functions
 * ========================================================== */

/**
 * @brief The print_content_eScreen() function print the content of the file in the window, do not change the cursor position
 *
 * @param screen eScreen pointer
 * @param first_line First line to print
 * @param number_length Number of digit of the higher line of the file
 */
void print_content_eScreen(eScreen *screen, eLine *first_line)
{
	eLine *current_line = first_line;
	size_t screen_pos = 0; /* y pos */
	int line_number = first_line->line_number;
	int number_length = screen->windows[FILE_LINESNUMBER]->width - 3;
	size_t width = screen->windows[FILE_CONTENT]->width;

	werase(screen->windows[FILE_CONTENT]->window);
	werase(screen->windows[FILE_LINESNUMBER]->window);

	wborder(screen->windows[FILE_LINESNUMBER]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);
	
	while(screen_pos < screen->windows[FILE_CONTENT]->height)
	{
		/* If there is at least one line left */
		if(current_line)
		{
			/* print line number */
			mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 1, "%*d ", number_length, line_number);

			/* print line */
			mvwaddstr(screen->windows[FILE_CONTENT]->window, screen_pos, 0, current_line->string);

			/* +1 because when end of line, put next file line two screen line after to let cursor go on next screen line*/
			screen_pos += current_line->length/width + 1;
			current_line = current_line->next;
		}

		/* If there are no lines left */
		else
		{
			mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 1, "%*c ", number_length, '~');
			screen_pos++;
		}
		line_number++;
	}
}


/**
 * @brief The create_file_window_eScreen() function allocate and initialize file windows.
 *
 * @param screen eScreen pointer
 * @param number_length number of digit in the lines number
 *
 */
void create_file_window_eScreen(eScreen *screen, unsigned int number_length)
{

	int width_file_linesnumber = 0, height_file_linesnumber = 0;
	int width_file_content = 0, height_file_content = 0;
	int x_file_linesnumber = 0, y_file_linesnumber = 0;
	int x_file_content = 0, y_file_content = 0;

	width_file_linesnumber = number_length + 3;
	height_file_linesnumber= screen->windows[FILE_BOX]->height - 2;
	
	width_file_content = screen->windows[FILE_BOX]->width - width_file_linesnumber - 2; 
	height_file_content = screen->windows[FILE_BOX]->height - 2;

	x_file_linesnumber = screen->windows[FILE_BOX]->x + 1;
	y_file_linesnumber = screen->windows[FILE_BOX]->y + 1;

	x_file_content = x_file_linesnumber + width_file_linesnumber;
	y_file_content = screen->windows[FILE_BOX]->y + 1;

	screen->windows[FILE_LINESNUMBER] = create_eWindow(height_file_linesnumber, width_file_linesnumber, y_file_linesnumber, x_file_linesnumber);
	screen->windows[FILE_CONTENT] = create_eWindow(height_file_content, width_file_content, y_file_content, x_file_content);
}	


/**
 * @brief The resize_file_window_eScreen() function resize file windows.
 *
 * @param screen eScreen pointer
 * @param number_length number of digit in the lines number
 *
 */
void resize_file_eScreen(eScreen *screen, unsigned int number_length)
{
	if(number_length+3 != screen->windows[FILE_LINESNUMBER]->width)
	{	
		int width_file_linesnumber = 0;
		int width_file_content = 0;
		int x_file_content = 0;

		width_file_linesnumber = number_length + 3;
		
		width_file_content = screen->windows[FILE_BOX]->width - width_file_linesnumber - 2; 

		x_file_content = screen->windows[FILE_LINESNUMBER]->x + width_file_linesnumber;

		
		screen->windows[FILE_LINESNUMBER]->width = width_file_linesnumber;
		screen->windows[FILE_CONTENT]->width = width_file_content;
		screen->windows[FILE_CONTENT]->x = x_file_content;

		wresize(screen->windows[FILE_LINESNUMBER]->window, screen->windows[FILE_LINESNUMBER]->height, width_file_linesnumber);
		mvwin(screen->windows[FILE_CONTENT]->window, screen->windows[FILE_CONTENT]->y, x_file_content);
		wresize(screen->windows[FILE_CONTENT]->window, screen->windows[FILE_CONTENT]->height, width_file_content);
	}
}


/* ==========================================================
 * eMenu functions
 * ========================================================== */

int add_item_menu_eScreen(eScree *screen, MENU_TYPE type, const char *item)
{
	return add_item_eMenu(screen->menus[type], item);
}


void next_item_menu_eScreen(eScree *screen, MENU_TYPE type)
{
	next_item_eMenu(screen->menus[type]);
}


void previous_item_menu_eScreen(eScree *screen, MENU_TYPE type)
{
	previous_item_eMenu(screen->menus[type]);
}
