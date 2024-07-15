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

#include "eManager.h"
#include "eScreen.h"
#include "eFile.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* Internal functions */
static bool process_input_eManager(eManager *manager, int input);

static bool process_DEFAULT_eManager(eManager *manager, int input);
static bool process_q_eManager(eManager *manager);
static bool process_w_eManager(eManager *manager);
static bool process_i_eManager(eManager *manager);
static bool process_d_eManager(eManager *manager);
static bool process_b_eManager(eManager *manager);
static bool process_ENTER_eManager(eManager *manager);
static bool process_ESCAPE_eManager(eManager *manager);
static bool process_BACKSPACE_eManager(eManager *manager);
static bool process_DELETE_eManager(eManager *manager);
static bool process_KEY_RIGHT_eManager(eManager *manager);
static bool process_KEY_LEFT_eManager(eManager *manager);
static bool process_KEY_DOWN_eManager(eManager *manager);
static bool process_KEY_UP_eManager(eManager *manager);

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

	manager->mode = DIR;	
	manager->screen = NULL;
	manager->file = NULL;
	manager->directory = NULL;
	manager->bar = NULL;

	return manager;
}


/**
 * @brief The delete_eManager() function deallocate and set the pointer to eManager structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eManager(eManager **manager)
{
	if(*manager == NULL)
		return;

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


/**
 * @brief The set_eBar_eManager() function set an eBar to eManager.
 *
 * @param manager eManager pointer
 * @param bar eBar pointer
 *
 */
void set_eBar_eManager(eManager *manager, eBar *bar)
{
	manager->bar = bar;
}


/**
 * @brief The set_eDirectory_eManager() function set an eDirectory to eManager.
 *
 * @param manager eManager pointer
 * @param directory eDirectory pointer
 *
 * @note This function also print the content of the repository in the screen.
 */
void set_eDirectory_eManager(eManager *manager, eDirectory *directory)
{
	manager->directory = directory;
}


/**
 * @brief The set_eFile_eManager() function set an eFile to eManager.
 *
 * @param manager eManager pointer
 * @param file eFile pointer
 *
 * @note This function also print the content of the file in the screen.
 */
int set_eFile_eManager(eManager *manager, eFile *file)
{
	// TODO: in the future, do not open in this function but in open_file_eBar
	if(open_eFile(file) == -1)
		return -1;

	manager->file = file;
	create_file_window_eScreen(manager->screen, digit_number(file->n_elines));
	print_content_eScreen(manager->screen, file->first_screen_line);
	
	return 0;
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

	if(result == false)
		return false;

	if(manager->mode == WRITE)
	{
		resize_file_eScreen(manager->screen, digit_number(manager->file->n_elines));
		print_content_eScreen(manager->screen, manager->file->first_screen_line);
		move_cursor_eScreen(manager->screen, gety_cursor_eManager(manager), getx_cursor_eManager(manager));
		update_file_eScreen(manager->screen);
	}
	else if(manager->mode == BAR)
	{
		update_bar_eScreen(manager->screen);
	}
	else if(manager->mode == DIR)
	{
		update_directory_eScreen(manager->screen);
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


		case 'b':
			return process_b_eManager(manager);


		case 'd':
			return process_d_eManager(manager);


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
		if(isprint(input) || input == '\t')
		{
			insert_char_eFile(manager->file, input);
			process_KEY_RIGHT_eManager(manager);
		}
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
	if(manager->mode == DIR || manager->mode == BAR)
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
	if(manager->mode == DIR || manager->mode == BAR)
	{
		if(manager->file != NULL)
		{
			if(manager->file->permissions != p_READWRITE)
			{
				/* TODO: WPOPUP */
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
 * @brief the process_d_input_eManager() function process a 'd' input 
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_d_eManager(eManager *manager)
{
	if(manager->mode == BAR)
	{
		manager->mode = DIR;
		set_current_window_eScreen(manager->screen, WDIR_BOX);
		set_current_menu_eScreen(manager->screen, MDIR);
	}
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'r');

	return true;
}


bool process_b_eManager(eManager *manager)
{
	if(manager->mode == DIR)
	{
		manager->mode = BAR;
		set_current_window_eScreen(manager->screen, WBAR_BOX);
		set_current_menu_eScreen(manager->screen, MBAR);
	}
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'b');

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
	if(manager->mode == DIR || manager->mode == BAR)
	{
		if(manager->file != NULL)
		{
			manager->mode = WRITE;
			set_current_window_eScreen(manager->screen, WFILE_CNT);
		}
	}
	
	else if(manager->mode == WRITE)
		process_DEFAULT_eManager(manager, 'i');

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
		manager->mode = DIR;
		set_current_window_eScreen(manager->screen, WDIR_BOX);
		set_current_menu_eScreen(manager->screen, MDIR);
		next_item_menu_eScreen(manager->screen);
	}

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
	 * 2. Remove string of current line after current pos 
	 * 3. Put the string of current line after current pos into buffer
	 * 4. Add an empty line
	 * 5. Set current pos in line to 0
	 * 6. Process KEY_DOWN
	 * 7. Insert buffer into new empty line
	 */
	if(manager->mode == WRITE)
	{
		buffer_length = sizeof(char)*(manager->file->current_line->length-manager->file->current_pos);
		buffer = malloc(buffer_length+1);
		memset(buffer, 0, buffer_length+1);

		buffer_length = get_string_eLine(manager->file->current_line, buffer, buffer_length, manager->file->current_pos);
		remove_string_eFile(manager->file, buffer_length);
		add_empty_line_eFile(manager->file, manager->file->current_line->line_number+1);
		manager->file->current_pos = 0;
		process_KEY_DOWN_eManager(manager);
		insert_string_eFile(manager->file, buffer, buffer_length);	
	
		free(buffer);	
		buffer = NULL;

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
	unsigned int line_number = 0;

	if(manager->mode == WRITE)
	{
		/* If current_pos > 0
		 * 1. Process KEY_LEFT 
		 * 2. Remove char before current_pos
		 */
		if(manager->file->current_pos > 0)
		{
			process_KEY_LEFT_eManager(manager);
			remove_char_eFile(manager->file);
		}
		/* If current_pos == 0 and current_line->previous exist
		 * 1. Allocate buffer
		 * 2. Put into buffer the current line
		 * 3. Save current line line_number 
		 * 4. Process KEY_LEFT to go to end of previous line
		 * 5. Insert buffer into the new current line
		 * 6. Delete 
		 */
		else if(manager->file->current_pos == 0 && manager->file->current_line->previous != NULL)
		{
			buffer_length = sizeof(char)*(manager->file->current_line->length-manager->file->current_pos);
			buffer = malloc(buffer_length+1);
			memset(buffer, 0, buffer_length+1);
			
			buffer_length = get_string_eLine(manager->file->current_line, buffer, buffer_length, manager->file->current_pos);
			line_number = manager->file->current_line->line_number;
			process_KEY_LEFT_eManager(manager);
			insert_string_eFile(manager->file, buffer, buffer_length);
			delete_line_eFile(manager->file, line_number);

			free(buffer);
			buffer = NULL;

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
		if(manager->file->current_pos < manager->file->current_line->length)
		{
			remove_char_eFile(manager->file);
		}
		else if( manager->file->current_pos >= manager->file->current_line->length && manager->file->current_line->next)
		{
			buffer_length = sizeof(char)*(manager->file->current_line->next->length);
			buffer = malloc(buffer_length+1);
			memset(buffer, 0, buffer_length+1);
			
			buffer_length = get_string_eLine(manager->file->current_line->next, buffer, buffer_length, 0);
			insert_string_eFile(manager->file, buffer, buffer_length);
			delete_line_eFile(manager->file, manager->file->current_line->next->line_number);
			
			free(buffer);
			buffer = NULL;
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
		if(manager->file->current_pos < manager->file->current_line->length)
		{
			manager->file->current_pos++;
		}
		else if(manager->file->current_pos >= manager->file->current_line->length && manager->file->current_line->next != NULL)
		{
			manager->file->current_pos = 0;
			process_KEY_DOWN_eManager(manager);
		}
	}
	else
	{
		next_item_menu_eScreen(manager->screen);
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
		if(manager->file->current_pos > 0)
		{
			manager->file->current_pos--;
		}
		else if(manager->file->current_pos == 0 && manager->file->current_line->previous != NULL)
		{
			manager->file->current_pos = manager->file->current_line->previous->length;
			process_KEY_UP_eManager(manager);
		}
	}
	else
	{
		previous_item_menu_eScreen(manager->screen);
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
		if(manager->file->current_line->next)
		{
			/* Do not get out of line with cursor  */
			if(manager->file->current_pos > manager->file->current_line->next->length)
				manager->file->current_pos = manager->file->current_line->next->length;				
			
			manager->file->current_line = manager->file->current_line->next;

			/* While cursor is out of screen (line to big), pull down the screen */
			while(gety_cursor_eManager(manager)+5 > get_height_eScreen(manager->screen, WFILE_CNT)-1)
				manager->file->first_screen_line = manager->file->first_screen_line->next;
		}
	}
	else
	{
		next_item_menu_eScreen(manager->screen);
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
		if(manager->file->current_line->previous)
		{
			/* Do not get out of line with cursor  */
			if(manager->file->current_pos > manager->file->current_line->previous->length)
				manager->file->current_pos = manager->file->current_line->previous->length;

			/* If current == first_line, pull up the screen */
			if(manager->file->current_line->line_number-5 < manager->file->first_screen_line->line_number && manager->file->first_screen_line->previous)
				manager->file->first_screen_line = manager->file->first_screen_line->previous;
				
			manager->file->current_line = manager->file->current_line->previous;
		}
	}
	else
	{
		previous_item_menu_eScreen(manager->screen);
	}

	return true;
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
	size_t width = get_width_eScreen(manager->screen, WFILE_CNT); 
	
	pos = screen_width_of_string(manager->file->current_line->string, manager->file->current_pos)%width;
	
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

	width = get_width_eScreen(manager->screen, WFILE_CNT);
	
	y=0;
	current = manager->file->first_screen_line;
	while(current && current != manager->file->current_line)
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
	
	y += screen_width_of_string(manager->file->current_line->string, manager->file->current_pos)/width;

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


int fill_dir_eManager(eManager *manager, eDirectory *directory, unsigned int level)
{
	unsigned int i=0;
	char *item = NULL;
	size_t alloc_item_size = 0;
	char *dirname = NULL;

	if(manager == NULL || directory == NULL)
		return -1;

	if(level == 0)
		dirname = directory->realpath;
	else
		dirname = directory->dirname;

	alloc_item_size = (strlen(dirname)+3+level*2);
	item = (char *) malloc(alloc_item_size*sizeof(char));
	memset(item, 0, alloc_item_size*sizeof(char));
	memset(item, ' ', level*2);
	strcat(item, "> ");
	strncat(item, dirname, strlen(dirname));
	add_item_menu_eScreen(manager->screen, MDIR, item);

	if(directory->is_open)
	{
		level++;
		for(i=0; i<directory->n_dirs; i++)
		{
			fill_dir_eManager(manager, directory->dirs[i], level);
		}
	
		for(i=0; i<directory->n_files; i++)
		{
			/* Char allocation */
			if(alloc_item_size <= strlen(directory->files[i]->filename))
			{
				alloc_item_size = (strlen(directory->files[i]->filename)+level*2+1);
				item = (char *) realloc(item, alloc_item_size*sizeof(char));
				if(item==NULL)
					return -1;
			}
			memset(item, 0, alloc_item_size*sizeof(char));
			memset(item, ' ', level*2);
			strncat(item, directory->files[i]->filename, strlen(directory->files[i]->filename));
			add_item_menu_eScreen(manager->screen, MDIR, item);
		}
	}	
	free(item);
	return 0;
}	
