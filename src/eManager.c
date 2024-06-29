/**
 * ===================================================
 * @file eManager.c
 * @brief Manager is the Controller of the MVC design
 * @author ALARY Dorian
 * @version 0.1
 * @date 27/06/2024
 * @copyright GNU Public License.
 *
 * @details This file contain every functions and variable relative to the eManager structure. 
 *          The functions manage the View (eScreen) and the model (eFile, eMenu, eBar)
 * ===================================================
 */

#include <stdlib.h>

#include "eManager.h"
#include "eScreen.h"
#include "eFile.h"


bool process_NORMAL_input_eManager(eManager *manager, int input);
bool process_WRITE_input_eManager(eManager *manager, int input);
bool process_WRITE_default_input_eManager(eManager *manager, int input);


eManager *create_eManager()
{
	eManager *manager;

	manager = (eManager *) malloc(sizeof(eManager));
	if(manager == NULL)
	{
		/* TODO: TRACE */
		return NULL;
	}

	manager->mode = NORMAL;	
	manager->screen = NULL;
	manager->file = NULL;
	manager->line = NULL;
	return manager;
}


void delete_eManager(eManager **manager)
{
	free(*manager);
	manager = NULL;

}


void set_eScreen_eManager(eManager *manager, eScreen *screen)
{
	manager->screen = screen;
}


//void set_menu_eManager(eManager *manager, eMenu *menu);
//void set_bar_eManager(eManager *manager, eBar *menu);


void set_eFile_eManager(eManager *manager, eFile *file)
{
	manager->file = file;
	manager->line = file->first;
}


bool run_eManager(eManager *manager)
{
	int input;
	bool result;

	input = getch();
	if(manager->mode == NORMAL)
		result = process_NORMAL_input_eManager(manager, input);

	else if(manager->mode == WRITE)
		result = process_WRITE_input_eManager(manager, input);

	else
		return false;

	return result;
}


bool process_NORMAL_input_eManager(eManager *manager, int input)
{
	bool result=true;

	switch(input)
	{
		/* Change mode to insertion mode */
		case 'i':
			if(manager->file)
			{
				manager->mode = WRITE;
				set_current_window_eScreen(manager->screen, FILE_CONTENT);
			}
			break;

		/* Quit app */
		case 'q':
			result = false;


		default:
			break;
	}
	return result;	
}

bool process_WRITE_input_eManager(eManager *manager, int input)
{
	bool result=true;

	switch(input)
	{
		/* escape */
		case 27:
			/* Change mode to normal */
			manager->mode = NORMAL;

			/* Set screen current window to menu */
			set_current_window_eScreen(manager->screen, MENU);
			break;
		

		default:
			result = process_WRITE_default_input_eManager(manager, input);
	}
	return result;
}


bool process_WRITE_default_input_eManager(eManager *manager, int input)
{
	insert_char_eLine(manager->line, input, manager->screen->current_window->x_cursor);
	insert_char_infile_eScreen(manager->screen, input);
	update_file_eScreen(manager->screen);
	return true;
}
