/**
 * @file 		eScreen.h
 * @brief 		eScreen Header
 * @author 		ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __ESCREEN_H__
#define __ESCREEN_H__

#include "eLine.h"
#include "eWindow.h"
#include "eMenu.h"


typedef struct {

	/** Number of columns */
	size_t width;

	/** Number of lines */
	size_t height;

	/** Array of every window */
	eWindow * windows[WINDOWS_NUMBER];

	/** Array of every menu */
	eMenu * menus[MENU_NUMBER];

} eScreen;


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
eScreen * create_eScreen(int n_lines, int n_cols);


/**
 * @brief The delete_eScreen() function deallocate the eScreen structure and set the pointer to the structure to NULL.
 *
 * @param manager: eManager pointer pointer
 */
void delete_eScreen(eScreen ** screen);


/**
 * @brief The update_repository_eScreen() function refresh the menu window.
 *
 * @param screen: eScreen pointer
 */
void update_directory_eScreen(eScreen * screen);


/*
 * @brief The update_bar_eScreen() function refresh the bar window.
 *
 * @param screen: eScreen pointer
 */
void update_bar_eScreen(eScreen * screen);


/**
 * @brief The update_file_eScreen() function refresh the file content and file number window.
 *
 * @param screen: eScreen pointer
 */
void update_file_eScreen(eScreen * screen);


/**
 * @brief The update_repository_eScreen() function refresh the menu window.
 *
 * @param screen: eScreen pointer
 */
void update_help_eScreen(eScreen * screen);


/**
 * @brief the update_all_eScreen() function refresh all the window of the screen.
 *
 * @param screen: eScreen pointer
 */
void update_all_eScreen(eScreen * screen);



/* ==========================================================
 * eWindow functions
 * ========================================================== */

/**
 * @brief The create_file_window_eScreen() function allocate and initialize file windows.
 *
 * @param screen: eScreen pointer
 * @param number_length: number of digit in the lines number
 */
void create_file_window_eScreen(eScreen * screen, unsigned int number_length);


// TODO: a faire sauter -> remplacer par resize_eWindow
/**
 * @brief The resize_file_window_eScreen() function resize file windows.
 *
 * @param screen: eScreen pointer
 * @param number_length: number of digit in the lines number
 */
void resize_file_eScreen(eScreen * screen, unsigned int number_length);


// TODO: A faire sauter -> remplacer par print_line_eWindow(eWindow *window, unsigned int x, unsigned int y, const char fmt, ...);
/**
 * @brief The print_content_eScreen() function print the content of the file in the window, do not change the cursor position
 *
 * @param screen: eScreen pointer
 * @param first_line: First line to print
 * @param number_length: Number of digit of the higher line of the file
 */
void print_content_eScreen(eScreen * screen, eLine const * first_line);


/**
 * @brief the move_cursor_eScreen() function move the cursor on the window designed by type.
 *
 * @param screen: eScreen pointer
 * @param y: y position
 * @param x: x position
 * @param type: Window type
 */
void move_cursor_eScreen(eScreen * screen, unsigned int y, unsigned int x, WINDOW_TYPE type);


/**
 * @brief the get_width_eSreen() return the width of the window pointed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 */
unsigned int get_width_eScreen(eScreen const * screen, WINDOW_TYPE type);


/**
 * @brief the get_height_eSreen() return the height of the window pointed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 */
unsigned int get_height_eScreen(eScreen const * screen, WINDOW_TYPE type);


 /**
 * @brief The get_input_eScreen() function request an input to the user.
 *
 * @param screen: eScreen pointer
 * @param type: Window type
 *
 * @return User input
 */
int get_input_eScreen(eScreen * screen, WINDOW_TYPE type);


/**
 * @brief The print_help() function print the array of string in the Help window. Last element of string array must be NULL.
 *
 * @param screen: eScreen pointer
 * @param string: string to display
 *
 * @note update_help_eScreen must be called.
 */
void print_help_eScreen(eScreen * screen, char const * const * string_array);


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
int add_item_menu_eScreen(eScreen * screen, MENU_TYPE type, char const * item);


/**
 * @brief The erase_menu_eScreen() function erase the virtual menu.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void erase_menu_eScreen(eScreen * screen, MENU_TYPE type);


/**
 * @brief The refresh_menu_eScreen() function refresh the menu designed by type. Refresh put virtual elements into physical elements.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void refresh_menu_eScreen(eScreen * screen, MENU_TYPE type);


/**
 * @brief The move_next_item_menu_eScreen() function do a next action on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_next_item_menu_eScreen(eScreen * screen, MENU_TYPE type);


/**
 * @brief The move_previous_item_menu_eScreen() function do a previous action on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_previous_item_menu_eScreen(eScreen * screen, MENU_TYPE type);


/**
 * @brief The move_current_item_menu_eScreen() function move the cursor on the current item of menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
void move_current_item_menu_eScreen(eScreen * screen, MENU_TYPE type);


/**
 * @brief The move_pattern_item_menu_eScreen() function move the cursor to the next match on the menu designed by type.
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 * @param pattern: pattern to match
 */
void move_pattern_item_menu_eScreen(eScreen * screen, MENU_TYPE type, char const * pattern);


/**
 * @brief The get_current_item_index_menu_eScreen() function return the index of the current item of the menu
 *
 * @param screen: eScreen pointer
 * @param type: Menu type
 */
int get_current_item_index_menu_eScreen(eScreen const * screen, MENU_TYPE type);


#endif
