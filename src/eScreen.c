/**
 * ===================================================
 * @file eScreen.c
 * @brief Contain eScreen structure and functions
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
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
 * @param n_lines: Number of lines of screen
 * @param n_cols: Number of columns in screen
 *
 * @return eScreen pointer or NULL if it was an error.
 *
 * @note delete_eScreen() must be called before exiting.
 */
eScreen *create_eScreen(int lines, int columns)
{
	eScreen *screen = NULL;
	int width_repository = 0, height_repository = 0;
	int width_bar = 0, height_bar = 0;
	int width_file_box = 0, height_file_box = 0;
	int width_help = 0, height_help = 0;
	
	screen = (eScreen *) malloc(sizeof(eScreen));
	if(screen == NULL)
	{
		return NULL;
	}

	/* Set eScreen dimension */
	screen->width = columns;
	screen->height = lines;
	
	/* Set WINDOWs dimension */
	width_help = columns;
	height_help = 4;

	width_repository = columns/5;
	height_repository = lines - height_help;

	width_bar = columns - width_repository;
	height_bar = lines/10;
	if(height_bar%2 ==0)
		height_bar++;
	if(height_bar < 3)
		height_bar = 3;

	width_file_box = width_bar;
	height_file_box = lines - height_bar - height_help;

	/* Create WINDOWs */
	screen->windows[WDIR_BOX] = create_eWindow(height_repository, width_repository, 0, 0);
	
	screen->windows[WDIR_ITEMS] = create_der_eWindow(screen->windows[WDIR_BOX], height_repository-2, width_repository-3, 1, 2);

	screen->windows[WBAR_BOX] = create_eWindow(height_bar, width_bar,0 , width_repository);
	
	screen->windows[WBAR_ITEMS] = create_der_eWindow(screen->windows[WBAR_BOX], 2, width_bar-2 , height_bar/2, 1);

	screen->windows[WFILE_BOX] = create_eWindow(height_file_box, width_file_box, height_bar, width_repository);

	screen->windows[WFILE_LNUM] = NULL; 

	screen->windows[WFILE_CNT] = NULL;

	screen->windows[WHELP] = create_eWindow(height_help, width_help, height_file_box + height_bar, 0);

	/* Create MENUs */
	screen->menus[MBAR] = create_eMenu(screen->windows[WBAR_BOX]->window, screen->windows[WBAR_ITEMS]->window, 1);
	screen->menus[MDIR] = create_eMenu(screen->windows[WDIR_BOX]->window, screen->windows[WDIR_ITEMS]->window, 0);

	return screen;
}


/**
 * @brief The delete_eScreen() function deallocate the eScreen structure and set the pointer to the structure to NULL.
 *
 * @param manager: eManager pointer pointer
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

	for(int i=0; i<MENU_NUMBER; i++)
	{
		if((*screen)->menus[i] != NULL)
		delete_eMenu(&(*screen)->menus[i]);
	}

	free(*screen);
	*screen = NULL;
}


/**
 * @brief The update_repository_eScreen() function refresh the menu window.
 *
 * @param screen: eScreen pointer
 */
void update_directory_eScreen(eScreen *screen)
{
	box(screen->windows[WDIR_BOX]->window, 0, 0);
	wrefresh(screen->windows[WDIR_BOX]->window);
	wrefresh(screen->windows[WDIR_ITEMS]->window);
}


/*
 * @brief The update_bar_eScreen() function refresh the bar window.
 *
 * @param screen: eScreen pointer
 */
void update_bar_eScreen(eScreen *screen)
{
	box(screen->windows[WBAR_BOX]->window, 0, 0);
	wrefresh(screen->windows[WBAR_BOX]->window);
	wrefresh(screen->windows[WBAR_ITEMS]->window);
}


/**
 * @brief The update_file_eScreen() function refresh the file content and file number window.
 *
 * @param screen: eScreen pointer
 */
void update_file_eScreen(eScreen *screen)
{
	wnoutrefresh(screen->windows[WFILE_LNUM]->window);
	wnoutrefresh(screen->windows[WFILE_CNT]->window);
	doupdate();
}


/**
 * @brief The update_repository_eScreen() function refresh the menu window.
 *
 * @param screen: eScreen pointer
 */
void update_help_eScreen(eScreen *screen)
{
	wrefresh(screen->windows[WHELP]->window);
}


/**
 * @brief the update_all_eScreen() function refresh all the window of the screen.
 *
 * @param screen: eScreen pointer
 */
void update_all_eScreen(eScreen *screen)
{
	box(screen->windows[WDIR_BOX]->window, 0, 0);
	box(screen->windows[WBAR_BOX]->window, 0, 0);
	box(screen->windows[WFILE_BOX]->window, 0, 0);
	for(int i=0 ; i<WINDOWS_NUMBER ; i++)
	{
		if(screen->windows[i] != NULL)
			wnoutrefresh(screen->windows[i]->window);
	}
	
	doupdate();
}


/* ==========================================================
 * eWindow functions
 * ========================================================== */

/**
 * @brief The create_file_window_eScreen() function allocate and initialize file windows.
 *
 * @param screen: eScreen pointer
 * @param number_length: number of digit in the lines number
 *
 */
void create_file_window_eScreen(eScreen *screen, unsigned int number_length)
{

	int width_file_linesnumber = 0, height_file_linesnumber = 0;
	int width_file_content = 0, height_file_content = 0;
	int x_file_linesnumber = 0, y_file_linesnumber = 0;
	int x_file_content = 0, y_file_content = 0;

	width_file_linesnumber = number_length + 3;
	height_file_linesnumber= screen->windows[WFILE_BOX]->height - 2;
	
	width_file_content = screen->windows[WFILE_BOX]->width - width_file_linesnumber - 2; 
	height_file_content = screen->windows[WFILE_BOX]->height - 2;

	x_file_linesnumber = screen->windows[WFILE_BOX]->x + 1;
	y_file_linesnumber = screen->windows[WFILE_BOX]->y + 1;

	x_file_content = x_file_linesnumber + width_file_linesnumber;
	y_file_content = screen->windows[WFILE_BOX]->y + 1;

	screen->windows[WFILE_LNUM] = create_eWindow(height_file_linesnumber, width_file_linesnumber, y_file_linesnumber, x_file_linesnumber);
	screen->windows[WFILE_CNT] = create_eWindow(height_file_content, width_file_content, y_file_content, x_file_content);
}	


/**
 * @brief The resize_file_window_eScreen() function resize file windows.
 *
 * @param screen: eScreen pointer
 * @param number_length: number of digit in the lines number
 */
void resize_file_eScreen(eScreen *screen, unsigned int number_length)
{
	if(number_length+3 != screen->windows[WFILE_LNUM]->width)
	{	
		int width_file_linesnumber = 0;
		int width_file_content = 0;
		int x_file_content = 0;

		width_file_linesnumber = number_length + 3;
		
		width_file_content = screen->windows[WFILE_BOX]->width - width_file_linesnumber - 2; 

		x_file_content = screen->windows[WFILE_LNUM]->x + width_file_linesnumber;

		
		screen->windows[WFILE_LNUM]->width = width_file_linesnumber;
		screen->windows[WFILE_CNT]->width = width_file_content;
		screen->windows[WFILE_CNT]->x = x_file_content;

		wresize(screen->windows[WFILE_LNUM]->window, screen->windows[WFILE_LNUM]->height, width_file_linesnumber);
		mvwin(screen->windows[WFILE_CNT]->window, screen->windows[WFILE_CNT]->y, x_file_content);
		wresize(screen->windows[WFILE_CNT]->window, screen->windows[WFILE_CNT]->height, width_file_content);
	}
}


/**
 * @brief The print_content_eScreen() function print the content of the file in the window, do not change the cursor position
 *
 * @param screen: eScreen pointer
 * @param first_line: First line to print
 * @param number_length: Number of digit of the higher line of the file
 */
void print_content_eScreen(eScreen *screen, eLine *first_line)
{
	eLine *current_line = first_line;
	size_t screen_pos = 0; /* y pos */
	int line_number = first_line->line_number;
	int number_length = screen->windows[WFILE_LNUM]->width - 3;
	size_t width = screen->windows[WFILE_CNT]->width;

	werase(screen->windows[WFILE_CNT]->window);
	werase(screen->windows[WFILE_LNUM]->window);

	wborder(screen->windows[WFILE_LNUM]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);
	
	while(screen_pos < screen->windows[WFILE_CNT]->height)
	{
		/* If there is at least one line left */
		if(current_line)
		{
			/* print line number */
			mvwprintw(screen->windows[WFILE_LNUM]->window, screen_pos, 1, "%*d ", number_length, line_number);

			/* print line */
			mvwaddstr(screen->windows[WFILE_CNT]->window, screen_pos, 0, current_line->string);

			/* +1 because when end of line, put next file line two screen line after to let cursor go on next screen line*/
			screen_pos += current_line->length/width + 1;
			current_line = current_line->next;
		}

		/* If there are no lines left */
		else
		{
			mvwprintw(screen->windows[WFILE_LNUM]->window, screen_pos, 1, "%*c ", number_length, '~');
			screen_pos++;
		}
		line_number++;
	}
}


/**
 * @brief the move_cursor_eScreen() function move the cursor on the window designed by type.
 *
 * @param screen: eScreen pointer
 * @param y: y position
 * @param x: x position
 * @param type: Window type
 */
void move_cursor_eScreen(eScreen *screen, unsigned int y, unsigned int x, WINDOW_TYPE type)
{
	wmove(screen->windows[type]->window, y, x);
}


/**
 * @brief the get_width_eSreen() return the width of the window pointed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 */
unsigned int get_width_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	return screen->windows[type]->width;
}


/**
 * @brief the get_height_eSreen() return the height of the window pointed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 */
unsigned int get_height_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	return screen->windows[type]->height;
}


/**
 * @brief The get_input_eScreen() function request an input to the user.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 *
 * @return User input
 */
int get_input_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	return wgetch(screen->windows[type]->window);
}


/**
 * @brief The print_help() function print the array of string in the Help window. Last element of string array must be NULL.
 *
 * @param screen: eScreen pointer
 * @param string: string to display
 *
 * @note update_help_eScreen must be called.
 */
void print_help_eScreen(eScreen *screen, char const * const * const string_array)
{
	if(screen == NULL)
		return;

	int i_elem = 0; /* Position of current elem in string arrray */
	int next_line = 1; /* In which line is the next element */
	int next_col = 1; /* In which col is the next element */
	size_t max_width_size_elem = 0; /* Max size taken by an element in the column */
	size_t remaining_size = screen->width - next_col - 1; /* Remaining size in lines */
	int n_line = screen->windows[WHELP]->height-2;

	werase(screen->windows[WHELP]->window);

	while(string_array[i_elem] != NULL)
	{
		if(max_width_size_elem < strlen(string_array[i_elem]))
			max_width_size_elem = strlen(string_array[i_elem]);

		if(max_width_size_elem > remaining_size)
			break;

		mvwaddstr(screen->windows[WHELP]->window, next_line, next_col, string_array[i_elem]);

		next_line++;
		if(next_line > n_line)
		{
			next_line = 1;
			next_col += max_width_size_elem+8;
			max_width_size_elem = 0;
		}
		i_elem++;
		remaining_size = screen->width - next_col - 1;
	}
}


/* ==========================================================
 * eMenu functions
 * ========================================================== */

/**
 * @brief The add_item_menu_eScreen() function add an item to the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 * @param item: item title
 *
 * @return 0 in success or -1 in failure.
 */
int add_item_menu_eScreen(eScreen *screen, MENU_TYPE type, const char *item)
{
	return add_item_eMenu(screen->menus[type], item);
}


/**
 * @brief The erase_menu_eScreen() function erase the virtual menu.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void erase_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	erase_eMenu(screen->menus[type]);
}


/**
 * @brief The refresh_menu_eScreen() function refresh the menu designed by type. Refresh put virtual elements into physical elements.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void refresh_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	refresh_eMenu(screen->menus[type]);
}


/**
 * @brief The move_next_item_menu_eScreen() function do a next action on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_next_item_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	move_next_item_eMenu(screen->menus[type]);
}


/**
 * @brief The move_previous_item_menu_eScreen() function do a previous action on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_previous_item_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	move_previous_item_eMenu(screen->menus[type]);
}


/**
 * @brief The move_current_item_menu_eScreen() function move the cursor on the current item of menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_current_item_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	move_current_item_eMenu(screen->menus[type]);
}


/**
 * @brief The move_pattern_item_menu_eScreen() function move the cursor to the next match on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 * @param pattern: pattern to match
 */
void move_pattern_item_menu_eScreen(eScreen *screen, MENU_TYPE type, const char *pattern)
{
	move_pattern_item_eMenu(screen->menus[type], pattern);
}


/**
 * @brief The get_current_item_index_menu_eScreen() function return the index of the current item of the menu
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
int get_current_item_index_menu_eScreen(eScreen *screen, MENU_TYPE type)
{
	return get_current_item_index_eMenu(screen->menus[type]);
}
