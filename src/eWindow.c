/**
 * ===================================================
 * @file eWindow.c
 * @brief eWindow is a View part of the MVC design
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 * @details This file contain every functions and variable relative to the eWindow structure. 
 *          The structure contain every information of a window.
 * ===================================================
 */

#include "eWindow.h"
#include <stdlib.h>


/**
 * @brief The create_eWindow() function allocate and initialize an eWindow structure.
 *
 * @param height: Window height (in lines)
 * @param width: Window width (in columns)
 * @param y: y position of the window in the Screen
 * @param x: x position of the winwow in the Screen
 *
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note delete_eWindow() must be called before exiting.
 */
eWindow * create_eWindow(size_t height, size_t width, unsigned int y, unsigned int x)
{
	eWindow *window = 0;

	window = (eWindow *) malloc(sizeof(eWindow));
	if(window == NULL)
	{
		return NULL;
	}

	window->orig = NULL;
	window->window = newwin(height, width, y, x);
	window->width = width;
	window->height = height;
	window->x = x;
	window->y = y;
	
	keypad(window->window, TRUE); /* activate KEY_UP, KEY_RIGHT, ... */

	return window;
}


/**
 * @brief The create_der_eWindow() function allocate and initialize an eWindow structure.
 *
 * @param orig: Window parent
 * @param height: Window height (in lines)
 * @param width: Window width (in columns)
 * @param y: y position of the window in the Screen
 * @param x: x position of the winwow in the Screen
 * 
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note y and x are relative to the origin of the eWindow orig.
 * @note delete_eWindow() must be called before exiting.
 */
eWindow * create_der_eWindow(eWindow * orig, size_t height, size_t width, unsigned int y, unsigned int x)
{
	if(orig == NULL)
		return NULL;

	eWindow *window = 0;

	window = (eWindow *) malloc(sizeof(eWindow));
	if(window == NULL)
		return NULL;

	window->orig = orig;
	window->window = derwin(orig->window, height, width, y, x);
	window->width = width;
	window->height = height;
	window->x = orig->x + x;
	window->y = orig->y + y;
	
	keypad(window->window, TRUE); /* activate KEY_UP, KEY_RIGHT, ... */

	return window;
}


/**
 * @brief The delete_eWindow() function deallocate the eWindow structure and set the pointer to the structure to NULL.
 *
 * @param window eWindow pointer pointer
 */
void delete_eWindow(eWindow ** window)
{
	if(*window == NULL)
		return;

	delwin((*window)->window);
	free(*window);
	*window = NULL;
}
