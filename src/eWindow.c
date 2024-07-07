/**
 * ===================================================
 * @file eWindow.c
 * @brief eWindow is a View part of the MVC design
 * @author ALARY Dorian
 * @version 0.1
 * @date 27/06/2024
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
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note delete_eWindow() must be called before exiting.
 */
eWindow *create_eWindow(size_t height, size_t width, unsigned int y, unsigned int x)
{
	eWindow *window = 0;

	window = (eWindow *) malloc(sizeof(eWindow));
	if(window == NULL)
	{
		return NULL;
	}

	window->window = newwin(height, width, y, x);
	window->width = width;
	window->height = height;
	window->x = x;
	window->y = y;
	
	keypad(window->window, TRUE); /* activate KEY_UP, KEY_RIGHT, ... */

	return window;
}


/**
 * @brief The delete_eWindow() function deallocate the eWindow structure and set the pointer to the structure to NULL.
 *
 * @param window eWindow pointer pointer
 */
void delete_eWindow(eWindow **window)
{
	delwin((*window)->window);
	free(*window);
	*window = NULL;
}
