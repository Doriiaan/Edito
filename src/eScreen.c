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

#include <stdlib.h>
#include <ncurses.h>

#include "eScreen.h"
#include "eLine.h"


/**
 * @brief The create_eScreen() function allocate and initialize an eScreen structure.
 *
 * @return eScreen pointer or NULL if it was an error, see logs.
 *
 * @note delete_eScreen() must be called before exiting.
 */
eScreen *create_eScreen(int lines, int columns)
{
	eScreen *screen;
	int width_menu, height_menu;
	int width_bar, height_bar;
	int width_file_box, height_file_box;
	int width_file_linesnumber, height_file_linesnumber;
	int width_file_content, height_file_content;

	screen = (eScreen *) malloc(sizeof(eScreen));
	if(screen == NULL)
	{
		return NULL;
	}

	/* Set eScreen dimension */
	screen->width = columns;
	screen->height = lines;
	
	/* Set WINDOWs dimension */
	width_menu = columns/5;
	height_menu = lines;

	width_bar = columns - width_menu;
	height_bar = lines/10;

	width_file_box = width_bar;
	height_file_box = lines - height_bar;
	
	width_file_linesnumber = 5; /* Default value, resize for each file */
	height_file_linesnumber=height_file_box - 2;
	
	width_file_content=columns - width_menu - width_file_linesnumber-2; 
	height_file_content=height_file_box - 2;

	/* Create WINDOWs */
	screen->windows[MENU] = create_eWindow(height_menu, width_menu, 0, 0);

	screen->windows[BAR] = create_eWindow(height_bar, width_bar,0 , width_menu);

	screen->windows[FILE_BOX] = create_eWindow(height_file_box, width_file_box, height_bar, width_menu);

	screen->windows[FILE_LINESNUMBER] = create_eWindow(height_file_linesnumber, width_file_linesnumber, height_bar+1,width_menu+1);

	screen->windows[FILE_CONTENT] = create_eWindow(height_file_content, width_file_content, height_bar+1, width_menu+1+width_file_linesnumber);

	screen->windows[POPUP] = NULL;

	screen->current_window = screen->windows[MENU];

	return screen;
}


/**
 * @brief The delete_eScreen() function deallocate the eScreen structure and set the pointer to the structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eScreen(eScreen **screen)
{
	delete_eWindow(&(*screen)->windows[MENU]);
	delete_eWindow(&(*screen)->windows[BAR]);
	delete_eWindow(&(*screen)->windows[FILE_BOX]);
	delete_eWindow(&(*screen)->windows[FILE_LINESNUMBER]);
	delete_eWindow(&(*screen)->windows[FILE_CONTENT]);
	if((*screen)->windows[POPUP])
		delete_eWindow(&(*screen)->windows[POPUP]);
	free(*screen);
	*screen = NULL;
}


/**
 * @brief The update_menu_eScreen() function refresh the menu window.
 *
 * @param screen eScreen pointer
 */
void update_menu_eScreen(eScreen *screen)
{
	box(screen->windows[MENU]->window, 0, 0);
	wnoutrefresh(screen->windows[MENU]->window);
	doupdate();
}


/*
 * @brief The update_bar_eScreen() function refresh the bar window.
 *
 * @param screen eScreen pointer
 */
void update_bar_eScreen(eScreen *screen)
{
	box(screen->windows[BAR]->window, 0, 0);
	wnoutrefresh(screen->windows[BAR]->window);
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
	box(screen->windows[MENU]->window, 0, 0);
	box(screen->windows[BAR]->window, 0, 0);
	box(screen->windows[FILE_BOX]->window, 0, 0);

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
	wmove(screen->current_window->window, screen->current_window->y_cursor, screen->current_window->x_cursor);
}


/**
 * @brief The print_content_eScreen() function print the content of the file in the window, do not change the cursor position
 *
 * @param screen eScreen pointer
 * @param first_line First line to print
 * @param number_length Number of digit of the higher line of the file
 */
void print_content_eScreen(eScreen *screen, eLine *first_line, unsigned int number_length)
{
	eLine *current_line = first_line;
	size_t screen_pos=0;
	int line_number=first_line->pos;
	
	/* Print the right border of the lines number window */
	wborder(screen->windows[FILE_LINESNUMBER]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);

	while(screen_pos < screen->windows[FILE_CONTENT]->height)
	{
		/* If there is at least one line left */
		if(current_line)
		{
			/* print line number */
			mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 0, "%*d", number_length, line_number);
			for(size_t i_part_of_line=0 ; i_part_of_line < current_line->length ; i_part_of_line+=screen->windows[FILE_CONTENT]->width)
			{
				/* print each part of line (if the line width is higher than the scren width) */
				mvwprintw(screen->windows[FILE_CONTENT]->window, screen_pos, 0, "%.*s", screen->windows[FILE_CONTENT]->width, current_line->string+i_part_of_line);
				screen_pos++;
			}
			current_line = current_line->next;
		}

		/* If there are no lines left */
		else
		{
			mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 0, "%.*s%c", number_length, " ", '~');
			screen_pos++;
		}
		line_number++;
	}

	/* Move the cursor to the current_window cursor position */
	wmove(screen->current_window->window, screen->current_window->y_cursor, screen->current_window->x_cursor);
}


/**
 * @brief The insert_char_eScreen() function insert a character in the current_window.
 *
 * @param screen eScreen pointer
 * @param ch Character to insert in the screen
 */
void insert_char_eScreen(eScreen *screen, char ch)
{
	waddch(screen->current_window->window, ch);
	getyx(screen->current_window->window, screen->current_window->y_cursor, screen->current_window->x_cursor);
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
