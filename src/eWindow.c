#include <stdlib.h>
#include <ncurses.h>

#include "eWindow.h"

eWindow *create_eWindow(size_t width, size_t height, unsigned int x, unsigned int y)
{
	eWindow *window;

	window = (eWindow *) malloc(sizeof(eWindow));
	if(window == NULL)
	{
		return NULL;
	}

	window->window = newwin(width, height, y, x);
	window->width = width;
	window->height = height;
	window->x = x;
	window->y = y;
	window->x_cursor = 0;
	window->y_cursor = 0;

	return window;
}


void delete_eWindow(eWindow **window)
{
	delwin((*window)->window);
	free(*window);
	*window = NULL;
}
