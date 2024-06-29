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
 *              - manage content of the screen
 * ===================================================
 */

#include <stdlib.h>
#include <ncurses.h>

#include "eScreen.h"
#include "eFile.h"

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

void update_menu_eScreen(eScreen *screen)
{
	box(screen->windows[MENU]->window, 0, 0);
	wnoutrefresh(screen->windows[MENU]->window);
	doupdate();
}


void update_bar_eScreen(eScreen *screen)
{
	box(screen->windows[BAR]->window, 0, 0);
	wnoutrefresh(screen->windows[BAR]->window);
	doupdate();
}


void update_file_eScreen(eScreen *screen)
{
	box(screen->windows[FILE_BOX]->window, 0, 0);
	wborder(screen->windows[FILE_LINESNUMBER]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);

	wnoutrefresh(screen->windows[FILE_BOX]->window);
	wnoutrefresh(screen->windows[FILE_LINESNUMBER]->window);
	wnoutrefresh(screen->windows[FILE_CONTENT]->window);
	doupdate();
}


void update_all_eScreen(eScreen *screen)
{
	box(screen->windows[MENU]->window, 0, 0);
	box(screen->windows[BAR]->window, 0, 0);
	box(screen->windows[FILE_BOX]->window, 0, 0);
	wborder(screen->windows[FILE_LINESNUMBER]->window, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);

	for(int i=0 ; i<WINDOWS_NUMBER ; i++)
	{
		wnoutrefresh(screen->windows[i]->window);
	}
	wnoutrefresh(screen->windows[POPUP]->window);
	
	doupdate();
}


void set_current_window_eScreen(eScreen *screen, WINDOW_TYPE type)
{
	screen->current_window = screen->windows[type];
}

void insert_char_infile_eScreen(eScreen *screen, int input)
{
	(void)screen;
	(void)input;
}
