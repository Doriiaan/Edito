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


/* Internal functions */
static bool process_input_eManager(eManager *manager, int input);

static bool process_DEFAULT_eManager(eManager *manager, int input);
static bool process_q_eManager(eManager *manager);
static bool process_w_eManager(eManager *manager);
static bool process_i_eManager(eManager *manager);
static bool process_ENTER_eManager(eManager *manager);
static bool process_ESCAPE_eManager(eManager *manager);
static bool process_BACKSPACE_eManager(eManager *manager);
static bool process_DELETE_eManager(eManager *manager);
static bool process_KEY_RIGHT_eManager(eManager *manager);
static bool process_KEY_LEFT_eManager(eManager *manager);
static bool process_KEY_DOWN_eManager(eManager *manager);
static bool process_KEY_UP_eManager(eManager *manager);

static int digit_number(unsigned int n);
unsigned int screen_width_of_string(const char *s, size_t length);


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
	*manager = NULL;
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
	create_file_window_eScreen(manager->screen, digit_number(file->n_elines));
	print_content_eScreen(manager->screen, file->first);
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
	int input = 0;
	bool result = false;

	curs_set(1);
	input = get_input_eScreen(manager->screen);
	curs_set(0);
	result =  process_input_eManager(manager, input);
	
	if(manager->mode == WRITE)
	{
		print_content_eScreen(manager->screen, manager->first_screen_line);
		move_cursor_eScreen(manager->screen, FILE_CONTENT, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
		update_file_eScreen(manager->screen);
	}
	else if(manager->mode == NORMAL)
	{
		move_cursor_eScreen(manager->screen, MENU, 1, 1);
		update_menu_eScreen(manager->screen);
	}

	return result;
}


/*
 * @brief the process_input_eManager() function process an input if the program is in WRITE mode.
 *
 * @param manager eManager pointer
 * @param input User input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_input_eManager(eManager *manager, int input)
{
	switch(input)
	{
		case 'q':
			return process_q_eManager(manager);


		case 'i':
			return process_i_eManager(manager);


		case 'w':
			return process_w_eManager(manager);


		/* ENTER */
		case '\n':
			return process_ENTER_eManager(manager);


		case KEY_UP:
			return process_KEY_UP_eManager(manager);

			
		case KEY_DOWN:
			return process_KEY_DOWN_eManager(manager);


		case KEY_LEFT:
			return process_KEY_LEFT_eManager(manager);


		case KEY_RIGHT:
			return process_KEY_RIGHT_eManager(manager);

		
		case KEY_BACKSPACE:
			return process_BACKSPACE_eManager(manager);
		
		
		case KEY_DC:
			return process_DELETE_eManager(manager);
		

		/* ESCAPE */
		case 27:
			return process_ESCAPE_eManager(manager);


		default:
			return process_DEFAULT_eManager(manager, input);
	}
}


/*
 * @brief the process_DEFAULT_input_eManager() function process a default input (character, number, ...)
 *
 * @param manager eManager pointer
 * @param input Default user input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_DEFAULT_eManager(eManager *manager, int input)
{
	if(manager->mode == WRITE)
	{
		insert_char_eLine(manager->current_line, input, manager->current_pos);
		process_KEY_RIGHT_eManager(manager);
	}
	return true;
}


/*
 * @brief the process_q_input_eManager() function process a 'q' input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_q_eManager(eManager *manager)
{
	if(manager->mode == NORMAL)
		return false;
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'q');

	return true;
}


/*
 * @brief the process_w_input_eManager() function process a 'w' input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_w_eManager(eManager *manager)
{
	if(manager->mode == NORMAL)
	{
		if(manager->file)
		{
			if(manager->file->permissions != p_READWRITE)
			{
				/* TODO: POPUP */
			}

			if(write_eFile(manager->file) == -1)
			{
				/* tmp file */
			}
		}
	}
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'w');

	return true;
}


/*
 * @brief the process_i_input_eManager() function process a 'i' input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_i_eManager(eManager *manager)
{
	if(manager->mode == NORMAL)
	{
		if(manager->file)
		{
			manager->mode = WRITE;
			set_current_window_eScreen(manager->screen, FILE_CONTENT);
		}
	}
	
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'i');

	return true;
}


/*
 * @brief the process_ENTER_input_eManager() function process a ENTER input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ENTER_eManager(eManager *manager)
{
	char *buffer = NULL;
	int buffer_length = 0;

	/*
	 * 1. Allocate buffer
	 * 2. Put into buffer, the string of current line after current pos
	 * 3. Add an empty line
	 * 4. Insert buffer into new empty line
	 * 5. Remove string of current line after current pos 
	 * 6. Set current pos to 0 and process KEY_DOWN
	 */
	if(manager->mode == WRITE)
	{
		buffer_length = sizeof(char)*(manager->current_line->length-manager->current_pos);
		buffer = malloc(buffer_length+1);
		memset(buffer, 0, buffer_length+1);

		buffer_length = get_string_eLine(manager->current_line, buffer, buffer_length, manager->current_pos);
		add_empty_line_eFile(manager->file, manager->current_line->pos+1);
		insert_string_eLine(manager->current_line->next, buffer, buffer_length, 0);	
		remove_string_eLine(manager->current_line, buffer_length, manager->current_pos);
	
		free(buffer);	
		buffer = NULL;
		manager->current_pos = 0;
		resize_file_eScreen(manager->screen, digit_number(manager->file->n_elines));
		process_KEY_DOWN_eManager(manager);
	}

	return true;
}


/*
 * @brief the process_ESCAPE_input_eManager() function process an ESCAPE input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_ESCAPE_eManager(eManager *manager)
{
	if(manager->mode == WRITE)
	{
		manager->mode = NORMAL;
		set_current_window_eScreen(manager->screen, MENU);
	}

	return true;
}


/*
 * @brief the process_BACKSPACE_input_eManager() function process a BACKSPACE input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_BACKSPACE_eManager(eManager *manager)
{
	char *buffer = NULL;
	int buffer_length = 0;
	unsigned int pos = 0;

	if(manager->mode == WRITE)
	{
		/* If current_pos > 0
		 * 1. Remove char before current_pos
		 * 2. Print content
		 * 3. Process KEY_LEFT 
		 */
		if(manager->current_pos > 0)
		{
			/* If current_pos > current_line->length (error), remove_char_eLine return -1, process just KEY_LEFT */
			remove_char_eLine(manager->current_line, manager->current_pos-1);
			process_KEY_LEFT_eManager(manager);
		}
		/* If current_pos == 0 and current_line->previous exist
		 * 1. Allocate buffer
		 * 2. Put into buffer the current line
		 * 3. Insert buffer into the previous line at the end of line
		 * 4. Save line pos
		 * 5. Process KEY_UP
		 * 6. Delete line at pos
		 * 7. Refresh screen
		 */
		else if(manager->current_pos == 0 && manager->current_line->previous != NULL)
		{
			buffer_length = sizeof(char)*(manager->current_line->length-manager->current_pos);
			buffer = malloc(buffer_length+1);
			memset(buffer, 0, buffer_length+1);

			buffer_length = get_string_eLine(manager->current_line, buffer, buffer_length, manager->current_pos);
			manager->current_pos = manager->current_line->previous->length;
			insert_string_eLine(manager->current_line->previous, buffer, buffer_length, manager->current_line->previous->length);
			free(buffer);
			buffer = NULL;

			pos = manager->current_line->pos;
			process_KEY_UP_eManager(manager);
			delete_line_eFile(manager->file, pos);
			resize_file_eScreen(manager->screen, digit_number(manager->file->n_elines));
		}
	}
	return true;
}


bool process_DELETE_eManager(eManager *manager)
{
	char *buffer = NULL;
	int buffer_length = 0;

	if(manager->mode == WRITE)
	{
		if(manager->current_pos < manager->current_line->length)
		{
			remove_char_eLine(manager->current_line, manager->current_pos);
		}
		else if( manager->current_pos >= manager->current_line->length && manager->current_line->next)
		{
			buffer_length = sizeof(char)*(manager->current_line->next->length);
			buffer = malloc(buffer_length+1);
			memset(buffer, 0, buffer_length+1);
			
			buffer_length = get_string_eLine(manager->current_line->next, buffer, buffer_length, 0);
			insert_string_eLine(manager->current_line, buffer, buffer_length, manager->current_line->length);
			free(buffer);
			buffer = NULL;
			
			delete_line_eFile(manager->file, manager->current_line->next->pos);
			resize_file_eScreen(manager->screen, digit_number(manager->file->n_elines));
		}

	}
	return true;
}

/*
 * @brief the process_KEY_RIGHT_input_eManager() function process a KEY_RIGHT input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_KEY_RIGHT_eManager(eManager *manager)
{
	if(manager->mode == WRITE)
	{
		if(manager->current_pos < manager->current_line->length)
		{
			manager->current_pos++;
		}
		else if(manager->current_pos >= manager->current_line->length && manager->current_line->next != NULL)
		{
			manager->current_pos = 0;
			process_KEY_DOWN_eManager(manager);
		}
	}
	return true;
}


/*
 * @brief the process_KEY_LEFT_input_eManager() function process a KEY_LEFT input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_KEY_LEFT_eManager(eManager *manager)
{
	if(manager->mode == WRITE)
	{
		if(manager->current_pos > 0)
		{
			manager->current_pos--;
		}
		else if(manager->current_pos == 0 && manager->current_line->previous != NULL)
		{
			manager->current_pos = manager->current_line->previous->length;
			process_KEY_UP_eManager(manager);
		}
	}
	return true;
}


/*
 * @brief the process_KEY_DOWN_input_eManager() function process a KEY_DOWN input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_KEY_DOWN_eManager(eManager *manager)
{
	if(manager->mode == WRITE)
	{
		if(manager->current_line->next)
		{
			/* Do not get out of line with cursor  */
			if(manager->current_pos > manager->current_line->next->length)
				manager->current_pos = manager->current_line->next->length;				
			
			manager->current_line = manager->current_line->next;

			/* While cursor is out of screen (line to big), pull down the screen */
			while(gety_cursor_eManager(manager)+5 > get_height_eScreen(manager->screen, FILE_CONTENT)-1)
				manager->first_screen_line = manager->first_screen_line->next;
		}
	}
	return true;
}


/*
 * @brief the process_KEY_UP_input_eManager() function process a KEY_UP input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_KEY_UP_eManager(eManager *manager)
{
	if(manager->mode == WRITE)
	{
		if(manager->current_line->previous)
		{
			/* Do not get out of line with cursor  */
			if(manager->current_pos > manager->current_line->previous->length)
				manager->current_pos = manager->current_line->previous->length;

			/* If current == first_line, pull up the screen */
			if(manager->first_screen_line->previous && manager->current_line->pos-5 < manager->first_screen_line->pos)
				manager->first_screen_line = manager->first_screen_line->previous;
				
			manager->current_line = manager->current_line->previous;
		}
	}

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


/*
 * @brief The getx_cursor_eManager() return the window file x position of cursor in fuction of file.
 *
 * @param manager eManager pointer
 *
 * @return returns x position.
 */
unsigned int getx_cursor_eManager(eManager *manager)
{
	unsigned int pos = 0;
	size_t width = get_width_eScreen(manager->screen, FILE_CONTENT); 
	
	pos = screen_width_of_string(manager->current_line->string, manager->current_pos)%width;
	
	return pos; 
}

/*
 * @brief The gety_cursor_eManager() return the window file y position of cursor in fuction of file.
 *
 * @param manager eManager pointer
 *
 * @return returns y position.
 */
unsigned int gety_cursor_eManager(eManager *manager)
{
	size_t width = 0;
	unsigned int y = 0;
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
			y += screen_width_of_string(current->string, current->length)/width;
			y += (screen_width_of_string(current->string, current->length) % width != 0) ? 1 : 0;
		}
		current = current->next;
	}
	
	y += screen_width_of_string(manager->current_line->string, manager->current_pos)/width;

	return y;
}


unsigned int screen_width_of_string(const char *s, size_t length)
{
	size_t real_length = strnlen(s, length);
	unsigned int i = 0;
	size_t width = 0;

	while(i < real_length)
	{
		if(s[i] == '\t')
			width += TABSIZE - width%TABSIZE;
		else
			width += 1;
		i++;
	}
	return width;
}
