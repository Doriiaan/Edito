/**
 * @file 		eScreen.h
 * @brief 		eScreen Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __ESCREEN_H__
#define __ESCREEN_H__

#include <ncurses.h>

#include "eLine.h"
#include "eWindow.h"

typedef struct {

	/* Calculate in columns */
	size_t width;

	/* Calculate in lines */
	size_t height;
	
	/* Array of every window */
	eWindow *windows[6]; 
	
	/* Array of every window */
	eWindow *current_window;

} eScreen;


/**
 * @brief The create_eScreen() function allocate and initialize an eScreen structure.
 *
 * @return eScreen pointer or NULL if it was an error, see logs.
 *
 * @note delete_eScreen() must be called before exiting.
 */
eScreen * create_eScreen(int n_lines, int n_cols);


/**
 * @brief The create_file_window_eScreen() function allocate and initialize file windows.
 *
 * @param screen eScreen pointer
 * @param number_length number of digit in the lines number
 *
 */
void create_file_window_eScreen(eScreen *screen, unsigned int number_length);


/**
 * @brief The delete_eScreen() function deallocate the eScreen structure and set the pointer to the structure to NULL.
 *
 * @param manager eManager pointer pointer
 */
void delete_eScreen(eScreen **screen);


/**
 * @brief The update_menu_eScreen() function refresh the menu window.
 *
 * @param screen eScreen pointer
 */
void update_menu_eScreen(eScreen *screen);


/*
 * @brief The update_bar_eScreen() function refresh the bar window.
 *
 * @param screen eScreen pointer
 */
void update_bar_eScreen(eScreen *screen);


/**
 * @brief The update_file_eScreen() function refresh the file content and file number window.
 *
 * @param screen eScreen pointer
 */
void update_file_eScreen(eScreen *screen);


/**
 * @brief the update_all_eScreen() function refresh all the window of the screen.
 *
 * @param screen eScreen pointer
 */
void update_all_eScreen(eScreen *screen);


/**
 * @brief The set_current_eScreen() function set the current window and cursor of the screen.
 *
 * @param type The type of the window (the type refers to the window)
 */
void set_current_window_eScreen(eScreen *screen, WINDOW_TYPE type);


/**
 * @brief The print_content_eScreen() function print the content of the file in the window, do not change the cursor position
 *
 * @param screen eScreen pointer
 * @param first_line First line to print
 * @param number_length Number of digit of the higher line of the file
 */
void print_content_eScreen(eScreen *screen, eLine *first_line);


/**
 * @brief The insert_char_eScreen() function insert a character in the current_window.
 *
 * @param screen eScreen pointer
 * @param ch Character to insert in the screen
 */
void insert_char_eScreen(eScreen *screen, char ch);


void move_cursor_eScreen(eScreen *screen, WINDOW_TYPE, unsigned int y, unsigned int x);


unsigned int get_width_eScreen(eScreen *screen, WINDOW_TYPE type);


unsigned int get_height_eScreen(eScreen *screen, WINDOW_TYPE type);


/**
 * @brief The get_input_eScreen() function request an input to the user.
 *
 * @param screen eScreen pointer
 */
int get_input_eScreen(eScreen *screen);

#endif
