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
#include <string.h>

#include "eManager.h"
#include "eScreen.h"
#include "eFile.h"


bool process_NORMAL_input_eManager(eManager *manager, int input);
bool process_WRITE_input_eManager(eManager *manager, int input);
bool process_WRITE_default_input_eManager(eManager *manager, int input);
int digit_number(unsigned int n);


/**
 * @brief The create_eManager() function allocate and initialize an eManager structure.
 *
 * @return eManager pointer or NULL if it was an error, see logs.
 *
 * @note delete_eManager() must be called before exiting.
 */
eManager *create_eManager()
{
	eManager *manager = NULL;

	manager = (eManager *) malloc(sizeof(eManager));
	if(manager == NULL)
	{
		/* TODO: TRACE */
		return NULL;
	}

	manager->mode = NORMAL;	
	manager->screen = NULL;
	manager->file = NULL;

	/* file cursor : TODO deplace it in eFile structure ?*/
	manager->current_line = NULL;
	manager->current_pos = 0;
	manager->first_screen_line = NULL;
	return manager;
}


/**
 * @brief The delete_eManager() function deallocate and set the pointer to eManager structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eManager(eManager **manager)
{
	free(*manager);
	manager = NULL;
}


/**
 * @brief The set_eScreen_eManager() function set a screen to eManager.
 *
 * @param manager eManager pointer
 * @param screen eScreen pointer
 */
void set_eScreen_eManager(eManager *manager, eScreen *screen)
{
	manager->screen = screen;
}


// TODO: void set_menu_eManager(eManager *manager, eMenu *menu);
// TODO: void set_bar_eManager(eManager *manager, eBar *menu);


/**
 * @brief The set_eFile_eManager() function set an eFile to eManager.
 *
 * @param manager eManager pointer
 * @param file eFile pointer
 *
 * @note This function also print the content of the file in the screen.
 */
void set_eFile_eManager(eManager *manager, eFile *file)
{
	manager->file = file;
	manager->current_line = file->first;
	manager->current_pos = 0;
	manager->first_screen_line = file->first;
	print_content_eScreen(manager->screen, file->first, digit_number(file->n_elines));
}


/**
 * @brief The run_eManager() function is the main function of eManager, this function call screen to get an input and process the input.
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool run_eManager(eManager *manager)
{
	int input;
	bool result;

	/* Get User input */
	input = get_input_eScreen(manager->screen);

	if(manager->mode == NORMAL)
		result = process_NORMAL_input_eManager(manager, input);

	else if(manager->mode == WRITE)
		result = process_WRITE_input_eManager(manager, input);

	else
		return false;

	return result;
}


/*
 * @brief the process_NORMAL_input_eManager() function process an input if the program is in NORMAL mode.
 *
 * @param manager eManager pointer
 * @param input User input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
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
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
				update_file_eScreen(manager->screen);
			}
			break;

		/* Write the file on disk */
		case 'w':
			if(manager->file)
			{
				if(manager->file->permissions != p_READWRITE)
				{
					/* TODO: POPUP */
					break;
				}

				if(write_eFile(manager->file) == -1)
				{
					/* TODO: TRACE */	
				}
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


/*
 * @brief the process_WRITE_input_eManager() function process an input if the program is in WRITE mode.
 *
 * @param manager eManager pointer
 * @param input User input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_WRITE_input_eManager(eManager *manager, int input)
{
	bool result = true;
	char *buffer = NULL;
	unsigned int buffer_length = 0;

	switch(input)
	{
		
		case KEY_UP:
			if(manager->current_line->previous)
			{
				if(manager->current_pos > manager->current_line->previous->length)
					manager->current_pos = manager->current_line->previous->length;

				if(manager->current_line == manager->first_screen_line)
					manager->first_screen_line = manager->first_screen_line->previous;
				
				manager->current_line = manager->current_line->previous;
				print_content_eScreen(manager->screen, manager->first_screen_line, digit_number(manager->file->n_elines));
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
				update_file_eScreen(manager->screen);
			}
			break;

		case '\n':
			buffer_length = sizeof(char)*(manager->current_line->length-manager->current_pos);
			buffer = malloc(buffer_length+1);
			memset(buffer, 0, buffer_length);

			buffer_length = get_string_eLine(manager->current_line, buffer, buffer_length, manager->current_pos);
		
			add_empty_line_eFile(manager->file, manager->current_line->pos+1);
			insert_string_eLine(manager->current_line->next, buffer, buffer_length, 0);
			remove_string_eLine(manager->current_line, manager->current_pos, buffer_length);
			
			free(buffer);	
			buffer = NULL;
			manager->current_pos = 0;
			/* Continue to KEY_DOWN */
			/* FALLTHRU */


		case KEY_DOWN:
			if(manager->current_line->next)
			{
				if(manager->current_pos > manager->current_line->next->length)
					manager->current_pos = manager->current_line->next->length;
				
				manager->current_line = manager->current_line->next;

				while(gety_cursor_eManager(manager) > get_height_eScreen(manager->screen, FILE_CONTENT)-1)
					manager->first_screen_line = manager->first_screen_line->next;

				print_content_eScreen(manager->screen, manager->first_screen_line, digit_number(manager->file->n_elines));
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
				update_file_eScreen(manager->screen);
			}
			break;


		case KEY_LEFT:
			if(manager->current_pos > 0)
			{
				manager->current_pos--;
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
			}
			break;


			case KEY_RIGHT:
			if(manager->current_pos < manager->current_line->length)
			{
				manager->current_pos++;
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
			}
			break;

		
		case KEY_BACKSPACE:
			if(manager->current_pos > 0 && manager->current_pos <= manager->current_line->length)
			{
				remove_char_eLine(manager->current_line, manager->current_pos-1);
				print_content_eScreen(manager->screen, manager->first_screen_line, digit_number(manager->file->n_elines));
				manager->current_pos--;
				move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
				update_file_eScreen(manager->screen);
			}
			break;
		
		
		/* escape */
		case 27:
			manager->mode = NORMAL;

			set_current_window_eScreen(manager->screen, MENU);
			move_cursor_eScreen(manager->screen, MENU, 1, 1);
			update_menu_eScreen(manager->screen);
			break;

		default:
			result = process_WRITE_default_input_eManager(manager, input);
			break;
	}
	return result;
}


/*
 * @brief the process_WRITE_default_input_eManager() function process a default input (character, number, ...) if the program is in WRITE mode.
 *
 * @param manager eManager pointer
 * @param input Default user input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_WRITE_default_input_eManager(eManager *manager, int input)
{
	insert_char_eLine(manager->current_line, input, manager->current_pos);
	print_content_eScreen(manager->screen, manager->first_screen_line, digit_number(manager->file->n_elines));
	manager->current_pos++;
	move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
	update_file_eScreen(manager->screen);
	return true;
}


/*
 * @brief The digit_number() function return the number of digit in a number.
 */
int digit_number(unsigned int n) 
{
    if (n < 10) 
		return 1;
    if (n < 100) 
		return 2;
    if (n < 1000) 
		return 3;
    if (n < 10000) 
		return 4;
    if (n < 100000) 
		return 5;
    if (n < 1000000) 
		return 6;
    if (n < 10000000) 
		return 7;
    if (n < 100000000) 
		return 8;
    if (n < 1000000000) 
		return 9;

    return 10;
}


unsigned int getx_cursor_eManager(eManager *manager)
{
	return manager->current_pos%get_width_eScreen(manager->screen, FILE_CONTENT);
}


unsigned int gety_cursor_eManager(eManager *manager)
{
	size_t width;
	unsigned int y;
	eLine *current = NULL;

	width = get_width_eScreen(manager->screen, FILE_CONTENT);
	
	y=0;
	current = manager->first_screen_line;
	while(current && current != manager->current_line)
	{
		if(current->length == 0)
			y++;
		else
		{
			y += current->length/width;
			y += (current->length%width != 0) ? 1 : 0;
		}
		current = current->next;
	}
	y += manager->current_pos/width;
	return y;
}
