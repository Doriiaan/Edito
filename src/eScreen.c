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
#include <string.h>

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
	eScreen *screen = NULL;
	int width_menu = 0, height_menu = 0;
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
	width_menu = columns/5;
	height_menu = lines;

	width_bar = columns - width_menu;
	height_bar = lines/10;

	width_file_box = width_bar;
	height_file_box = lines - height_bar;
	

	/* Create WINDOWs */
	screen->windows[MENU] = create_eWindow(height_menu, width_menu, 0, 0);

	screen->windows[BAR] = create_eWindow(height_bar, width_bar,0 , width_menu);

	screen->windows[FILE_BOX] = create_eWindow(height_file_box, width_file_box, height_bar, width_menu);

	screen->windows[FILE_LINESNUMBER] = NULL; 

	screen->windows[FILE_CONTENT] = NULL;

	screen->windows[POPUP] = NULL;

	screen->current_window = screen->windows[MENU];

	return screen;
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

	width_file_linesnumber = number_length + 2;
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
}


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
	int line_number = first_line->pos;
	char *refresh_line = NULL;
	bool refresh_last_line = true;
	size_t i_part_of_line=0;
	int number_length = screen->windows[FILE_LINESNUMBER]->width - 2;

	refresh_line = (char *) malloc((screen->windows[FILE_CONTENT]->width+1)*sizeof(char));
	memset(refresh_line, ' ', screen->windows[FILE_CONTENT]->width);
	refresh_line[screen->windows[FILE_CONTENT]->width] = 0;

	/* Print the right border of the lines number window */
	wborder(screen->windows[FILE_LINESNUMBER]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);


	while(screen_pos < screen->windows[FILE_CONTENT]->height)
	{
		/* If there is at least one line left */
		if(current_line)
		{
			/* print line number */
			i_part_of_line=0;
			
			/* do while, because, do exact same thing when current_line->length == 0 */
			do
			{
				if(i_part_of_line == 0)
					mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 1, "%*d", number_length, line_number);
				else
					mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 1, "%*c", number_length, ' ');
				
				/* refresh line */
				mvwprintw(screen->windows[FILE_CONTENT]->window, screen_pos, 0, "%s", refresh_line);

				/* print line */
				mvwprintw(screen->windows[FILE_CONTENT]->window, screen_pos, 0, "%.*s", screen->windows[FILE_CONTENT]->width, current_line->string+i_part_of_line);

				i_part_of_line+=screen->windows[FILE_CONTENT]->width;
				
				screen_pos++;
			}while(i_part_of_line < current_line->length);
			
			current_line = current_line->next;
		}

		/* If there are no lines left */
		else
		{
			/* refresh last line */
			if(refresh_last_line)
			{
				mvwprintw(screen->windows[FILE_CONTENT]->window, screen_pos, 0, "%s", refresh_line);
				refresh_last_line = false;
			}

			mvwprintw(screen->windows[FILE_LINESNUMBER]->window, screen_pos, 1, "%*c", number_length, '~');
			screen_pos++;
		}
		line_number++;
	}
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


/**
 * @brief The get_input_eScreen() function request an input to the user.
 *
 * @param screen eScreen pointer
 */
int get_input_eScreen(eScreen *screen)
{
	return wgetch(screen->current_window->window);
}
