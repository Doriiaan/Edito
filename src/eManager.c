/**
 * ===================================================
 * @file eManager.c
 * @brief Manager is the Controller of the MVC design
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
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

#define CTRL(x) (x & 0x1F)

/* Internal functions */
static bool process_input_eManager(eManager * manager, int input);

static bool process_DEFAULT_eManager(eManager * manager, int input);
static bool process_ctrlq_eManager(eManager * manager);
static bool process_ctrls_eManager(eManager * manager);
static bool process_ctrlf_eManager(eManager * manager);
static bool process_ctrld_eManager(eManager * manager);
static bool process_ctrlb_eManager(eManager * manager);
static bool process_ENTER_eManager(eManager * manager);
static bool process_ESCAPE_eManager(eManager * manager);
static bool process_BACKSPACE_eManager(eManager * manager);
static bool process_DELETE_eManager(eManager * manager);
static bool process_KEY_RIGHT_eManager(eManager * manager);
static bool process_KEY_LEFT_eManager(eManager *manager);
static bool process_KEY_DOWN_eManager(eManager *manager);
static bool process_KEY_UP_eManager(eManager *manager);

static void change_mode_eManager(eManager * manager, MODE mode);
static unsigned int screen_width_of_string(char const * s, size_t length);
static void add_help_msg_eManager(eManager * manager, char const * message);


/* CONSTANTS */
char const * const DEFAULT_HELP_MESSAGE[sizeof(MODE)][6] =
{
	/* DIR */
	{
		"Ctrl+Q: Quit",
		"Ctrl+B: Bar",
		"Ctrl+F: File",
		"^ / v : UP / DOWN",
		"Enter: Open file / dir",
		NULL
	},

	/* WRITE */
	{
		"Ctrl+Q: Quit",
		"Ctrl+D: Directory",
		"Ctrl+B: BAR",
		"Ctrl+S: Save file",
		NULL
	},

	/* BAR */
	{
		"Ctrl+Q: Quit",
		"Ctrl+D: Directory",
		"Ctrl+F: File",
		"<- / -> : Left / Right",
		"Enter: Open file",
		NULL
	}
};


/**
 * @brief The create_eManager() function allocate and initialize an eManager structure.
 *
 * @return eManager pointer or NULL if it was an error, see logs.
 *
 * @note delete_eManager() must be called before exiting.
 */
eManager * create_eManager(void)
{
	eManager *manager = NULL;

	manager = (eManager *) malloc(sizeof(eManager));
	if(manager == NULL)
	{
		return NULL;
	}

	manager->lastmode = DIR;
	manager->mode = DIR;
	manager->screen = NULL;
	manager->file = NULL;
	manager->directory = NULL;
	manager->bar = NULL;
	manager->help_msg = NULL;

	return manager;
}


/**
 * @brief The delete_eManager() function deallocate and set the pointer to eManager structure to NULL.
 *
 * @param manager: eManager pointer pointer
 */
void delete_eManager(eManager ** manager)
{
	if(*manager == NULL)
		return;

	free(*manager);
	*manager = NULL;
}


/**
 * @brief The set_eScreen_eManager() function set a screen to eManager.
 *
 * @param manager: eManager pointer
 * @param screen: eScreen pointer
 */
void set_eScreen_eManager(eManager * manager, eScreen * screen)
{
	manager->screen = screen;
}


/**
 * @brief The set_eBar_eManager() function set an eBar to eManager.
 *
 * @param manager: eManager pointer
 * @param bar: eBar pointer
 */
void set_eBar_eManager(eManager *manager, eBar * bar)
{
	manager->bar = bar;
}


/**
 * @brief The set_eDirectory_eManager() function set an eDirectory to eManager.
 *
 * @param manager: eManager pointer
 * @param directory: eDirectory pointer
 *
 * @note This function also print the content of the repository in the screen.
 */
void set_eDirectory_eManager(eManager * manager, eDirectory * directory)
{
	manager->directory = directory;
}


/**
 * @brief The set_eFile_eManager() function set an eFile to eManager.
 *
 * @param manager: eManager pointer
 * @param file: eFile pointer
 */
void set_eFile_eManager(eManager * manager, eFile * file)
{
	manager->file = file;
}


/**
 * @brief The run_eManager() function is the main function of eManager, this function call screen to get an input and process the input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool run_eManager(eManager * manager)
{
	int input = 0;
	bool result = false;

	/* Get input */
	curs_set(1);
	if(manager->mode == WRITE)
		input = get_input_eScreen(manager->screen, WFILE_BOX);
	else if(manager->mode == DIR)
		input = get_input_eScreen(manager->screen, WDIR_BOX);
	else if(manager->mode == BAR)
		input = get_input_eScreen(manager->screen, WBAR_BOX);
	curs_set(0);

	/* Process input */
	result =  process_input_eManager(manager, input);

	if(result == false)
		return false;

	/* Update screen */
	send_help_msg_to_screen_eManager(manager);
	update_help_eScreen(manager->screen);

	if(manager->mode == WRITE)
	{
		resize_file_eScreen(manager->screen, digit_number(manager->file->n_elines));
		print_content_eScreen(manager->screen, manager->file->first_screen_line);
		move_cursor_eScreen(manager->screen, gety_cursor_eManager(manager), getx_cursor_eManager(manager), WFILE_CNT);
		update_file_eScreen(manager->screen);
	}
	else if(manager->mode == BAR)
	{
		move_current_item_menu_eScreen(manager->screen, MBAR);
		update_bar_eScreen(manager->screen);
	}
	else if(manager->mode == DIR)
	{
		move_current_item_menu_eScreen(manager->screen, MDIR);
		update_directory_eScreen(manager->screen);
	}

	return result;
}


/*
 * @brief the process_input_eManager() function process an input if the program is in WRITE mode.
 *
 * @param manager: eManager pointer
 * @param input: User input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_input_eManager(eManager * manager, int input)
{
	switch(input)
	{
		/* exit */
		case CTRL('q'):
			return process_ctrlq_eManager(manager);

		/* Enter file */
		case CTRL('f'):
			return process_ctrlf_eManager(manager);

		/* Enter bar */
		case CTRL('b'):
			return process_ctrlb_eManager(manager);

		/* Enter directory */
		case CTRL('d'):
			return process_ctrld_eManager(manager);


		case CTRL('s'):
			return process_ctrls_eManager(manager);


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
 * @brief the process_DEFAULT_input_eManager() function process a default input (character, number, ...).
 *
 * @param manager: eManager pointer
 * @param input: Default user input to process
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_DEFAULT_eManager(eManager * manager, int input)
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
 * @brief the process_ctrlq_input_eManager() function process a CTRLE input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ctrlq_eManager(eManager * manager)
{
	(void)manager;
	return false;
}


/*
 * @brief the process_ctrls_input_eManager() function process a CTRLS input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ctrls_eManager(eManager * manager)
{
	if(manager->file != NULL && manager->mode == WRITE)
	{
		if(manager->file->permissions != p_READWRITE)
		{
			add_help_msg_eManager(manager, "Readonly file.");
		}
		else if(write_eFile(manager->file) == -1)
		{
			/* tmp file */
		}
		add_help_msg_eManager(manager, "File saved.");
	}

	return true;
}


/*
 * @brief the process_ctrld_input_eManager() function process a CTRLD input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ctrld_eManager(eManager * manager)
{
	change_mode_eManager(manager, DIR);

	return true;
}


/*
 * @brief the process_ctrlb_input_eManager() function process a CTRLB input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ctrlb_eManager(eManager * manager)
{
	if(count_eBar(manager->bar) != 0)
		change_mode_eManager(manager, BAR);
	else
		add_help_msg_eManager(manager, "No files open.");

	return true;
}


/*
 * @brief the process_ctrlf_input_eManager() function process a CTRLF input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ctrlf_eManager(eManager * manager)
{
	if(manager->file != NULL)
		change_mode_eManager(manager, WRITE);
	else
		add_help_msg_eManager(manager, "No files open.");

	return true;
}


/*
 * @brief the process_ESCAPE_input_eManager() function process an ESCAPE input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_ESCAPE_eManager(eManager * manager)
{
	change_mode_eManager(manager, manager->lastmode);

	return true;
}


/*
 * @brief the process_ENTER_input_eManager() function process a ENTER input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_ENTER_eManager(eManager * manager)
{
	char *buffer = NULL;
	int buffer_length = 0;
	int item_index = 0;
	eFile *file = NULL;
	eDirectory *directory = NULL;

	/*  */
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
	else if(manager->mode == DIR)
	{
		item_index = get_current_item_index_menu_eScreen(manager->screen, MDIR);
		get_item_at_index_eDirectory(manager->directory, item_index, &directory, &file);
		if(directory != NULL)
		{
			/* close the directory and delete dirs/files from menu */
			if(directory->is_open)
			{
				directory->is_open = false;
			}
			else
			{
				directory->is_open = true;
			}
			fill_directory_menu_eManager(manager, manager->directory, 0);
			refresh_menu_eScreen(manager->screen, MDIR);
		}
		else if(file != NULL)
		{
			/* If file isn't in the bar */
			if(!is_file_in_eBar(manager->bar, file))
			{
				/* Try to open the file */
				if(open_eFile(file) == -1)
				{
					add_help_msg_eManager(manager, "Impossible to open file.");
					return true;
				}
				if(file->permissions == p_READONLY)
					add_help_msg_eManager(manager, "Readonly file.");


				/* Add file to eBar or quit, adding file to eBar */
				if(add_file_eBar(manager->bar, file) == -1)
					return true;

				/* Add filename to the bar menu */
				buffer_length = strlen(file->filename)+1;
				buffer = (char *) malloc(buffer_length*sizeof(char));
				memset(buffer, 0, buffer_length);
				strcpy(buffer, file->filename);

				/* Add item to the menu, and refresh the window */
				add_item_menu_eScreen(manager->screen, MBAR, buffer);
				refresh_menu_eScreen(manager->screen, MBAR);

				/* Deplace cursor to the file in the menu bar */
				move_pattern_item_menu_eScreen(manager->screen, MBAR, file->filename);
				update_bar_eScreen(manager->screen);

				/* Create or resize file Window for the file (resize for lines number) */
				if(manager->screen->windows[WFILE_CNT] == NULL)
					create_file_window_eScreen(manager->screen, file->n_elines);
				else
					resize_file_eScreen(manager->screen, file->n_elines);

				/* Enter write mode */
				set_eFile_eManager(manager, file);
				change_mode_eManager(manager, WRITE);
			}
			/* The file is in the bar */
			else
			{
				/* Deplace cursor to the file in the menu bar */
				move_pattern_item_menu_eScreen(manager->screen, MBAR, file->filename);
				update_bar_eScreen(manager->screen);
				resize_file_eScreen(manager->screen, file->n_elines);

				/* Enter write mode */
				set_eFile_eManager(manager, file);
				change_mode_eManager(manager, WRITE);
			}
		}
	}
	else if(manager->mode == BAR)
	{
		item_index = get_current_item_index_menu_eScreen(manager->screen, MBAR);
		file = (eFile *) get_file_eBar(manager->bar, item_index);

		/* Enter write mode */
		set_eFile_eManager(manager, file);
		change_mode_eManager(manager, WRITE);
	}

	return true;
}


/*
 * @brief the process_BACKSPACE_input_eManager() function process a BACKSPACE input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_BACKSPACE_eManager(eManager * manager)
{
	char *buffer = NULL;
	int buffer_length = 0;
	unsigned int line_number = 0;

	if(manager->mode == WRITE)
	{
		/* In the middle of a line, process move left and remove current char */
		if(manager->file->current_pos > 0)
		{
			process_KEY_LEFT_eManager(manager);
			remove_char_eFile(manager->file);
		}
		/* If cursor at beginning of line, put current_line into previous line  */
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


/*
 * @brief the process_DELETE_input_eManager() function process a DELETE input.
 *
 * @param manager: eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_DELETE_eManager(eManager * manager)
{
	char *buffer = NULL;
	int buffer_length = 0;

	if(manager->mode == WRITE)
	{
		/* In the middle of a line, remove current char */
		if(manager->file->current_pos < manager->file->current_line->length)
		{
			remove_char_eFile(manager->file);
		}
		/* If end of line, put next line into current line */
		else if(manager->file->current_pos >= manager->file->current_line->length && manager->file->current_line->next)
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

bool process_KEY_RIGHT_eManager(eManager * manager)
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
	/* Do not look good, but its ok */
	else if(manager->mode == DIR)
		move_next_item_menu_eScreen(manager->screen, MDIR);
	else if(manager->mode == BAR)
		move_next_item_menu_eScreen(manager->screen, MBAR);

	return true;
}


/*
 * @brief the process_KEY_LEFT_input_eManager() function process a KEY_LEFT input
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_KEY_LEFT_eManager(eManager * manager)
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
	else if(manager->mode == DIR)
		move_previous_item_menu_eScreen(manager->screen, MDIR);
	else if(manager->mode == BAR)
		move_previous_item_menu_eScreen(manager->screen, MBAR);

	return true;
}


/*
 * @brief the process_KEY_DOWN_input_eManager() function process a KEY_DOWN input
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */

bool process_KEY_DOWN_eManager(eManager * manager)
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
	else if(manager->mode == DIR)
		move_next_item_menu_eScreen(manager->screen, MDIR);
	else if(manager->mode == BAR)
		move_next_item_menu_eScreen(manager->screen, MBAR);
	return true;
}


/*
 * @brief the process_KEY_UP_input_eManager() function process a KEY_UP input
 *
 * @param manager eManager pointer
 *
 * @return returns true if the program continues and false otherwise.
 */
bool process_KEY_UP_eManager(eManager * manager)
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
	else if(manager->mode == DIR)
		move_previous_item_menu_eScreen(manager->screen, MDIR);
	else if(manager->mode == BAR)
		move_previous_item_menu_eScreen(manager->screen, MBAR);

	return true;
}


/*
 * @brief The getx_cursor_eManager() return the window file x position of cursor in fuction of file.
 *
 * @param manager: eManager pointer
 *
 * @return returns x position.
 */
unsigned int getx_cursor_eManager(eManager const * manager)
{
	unsigned int pos = 0;
	size_t width = get_width_eScreen(manager->screen, WFILE_CNT);

	pos = screen_width_of_string(manager->file->current_line->string, manager->file->current_pos)%width;

	return pos;
}


/*
 * @brief The gety_cursor_eManager() return the window file y position of cursor in fuction of file.
 *
 * @param manager: eManager pointer
 *
 * @return returns y position.
 */
unsigned int gety_cursor_eManager(eManager const * manager)
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


/**
 * @brief The screen_wifth_of_string() function return the size in terminal cell of string.
 *
 * @param s: string
 * @param length: string length in character
 *
 * @return the size in terminal cell of string.
 */
unsigned int screen_width_of_string(char const * s, size_t length)
{
	size_t real_length = strnlen(s, length);
	unsigned int i = 0;
	size_t width = 0;

	while(i < real_length)
	{
		if(s[i] == '\t')
			width += TABSIZE - width%TABSIZE;

		else
			width++;
		i++;
	}
	return width;
}


/**
 * The fill_directory_menu_eManager() function fill the directory screen menu depending on the manager directory.
 *
 * @param manager: eManager pointer
 * @param directory: pointer on the root directory (for recursivity)
 * @param level: level of the directory. The first must be 0
 *
 * @return 0 on success or -1 in failure.
 * @note This is a recursive function.
 */
int fill_directory_menu_eManager(eManager const * manager, eDirectory const * directory, unsigned int level)
{
	unsigned int i=0;
	char *item = NULL;
	size_t alloc_item_size = 0;
	char *dirname = NULL;

	if(manager == NULL || directory == NULL)
		return -1;

	/* Root element show path and erase all menu*/
	if(level == 0)
	{
		erase_menu_eScreen(manager->screen, MDIR);
		dirname = directory->realpath;
	}
	else
		dirname = directory->dirname;

	/* Directory title creation */
	alloc_item_size = (strlen(dirname)+3+level*2);
	item = (char *) malloc(alloc_item_size*sizeof(char));
	memset(item, 0, alloc_item_size*sizeof(char));
	memset(item, ' ', level*2);
	if(directory->is_open)
		strcat(item, "v ");
	else
		strcat(item, "> ");
	strncat(item, dirname, strlen(dirname));
	add_item_menu_eScreen(manager->screen, MDIR, item);

	/* If directory is open in directory menu, display its element */
	if(directory->is_open)
	{
		level++;

		/* Recursive on all directory */
		for(i=0; i<directory->n_dirs; i++)
		{
			fill_directory_menu_eManager(manager, directory->dirs[i], level);
		}

		/* Display directory files */
		for(i=0; i<directory->n_files; i++)
		{
			/* File title allocation */
			if(alloc_item_size <= strlen(directory->files[i]->filename)+2*level+1)
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


/**
 * @brief The change_mode_eManager() function change the mode of the manager and save current mode in last mode.
 *
 * @param manager: eManager pointer
 * @param mode: eManager mode
 */
void change_mode_eManager(eManager * manager, MODE mode)
{
	manager->lastmode = manager->mode;
	manager->mode = mode;
}


/**
 * @brief The add_help_msg_eManager() function set the next help message. If there is already a message, the message isn't modified.
 *
 * @param manager: eManager pointer
 * @param message: Message to set
 */
static void add_help_msg_eManager(eManager * manager, char const * message)
{
	/* If manager is NULL or if the manager already has a help message */
	if(manager == NULL || manager->help_msg != NULL)
		return;

	manager->help_msg = (char *) malloc((strlen(message)+1)*sizeof(char));
	memset(manager->help_msg, 0, strlen(message)+1);

	strncpy(manager->help_msg, message, strlen(message)+1);
}


/**
 * @brief The send_help_msg_to_screen_eManager() function send help message to the screen.
 *
 * @note If no message is set, send default help message.
 *
 * @param manager: eManager pointer
 */
void send_help_msg_to_screen_eManager(eManager * manager)
{
	char const ** string_array = NULL;

	if(manager == NULL || manager->screen == NULL)
		return;

	/* if there is no message, send default message */
	if(manager->help_msg == NULL)
	{
		print_help_eScreen(manager->screen, DEFAULT_HELP_MESSAGE[manager->mode]);
	}
	else
	{
		string_array = (char const **) malloc(2*sizeof(char const *));
		string_array[0] = manager->help_msg;
		string_array[1] = NULL;

		print_help_eScreen(manager->screen, string_array);

		free(manager->help_msg);
		manager->help_msg = NULL;
		free(string_array);
		string_array = NULL;
	}
}
