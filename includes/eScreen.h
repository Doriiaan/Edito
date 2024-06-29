#ifndef __ESCREEN_H__
#define __ESCREEN_H__

#include <ncurses.h>

#include "eFile.h"
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

eScreen * create_eScreen(int n_lines, int n_cols);
void delete_eScreen(eScreen **screen);

void update_menu_eScreen(eScreen *screen);
void update_bar_eScreen(eScreen *screen);
void update_file_eScreen(eScreen *screen);
void update_all_eScreen(eScreen *screen);

void set_current_window_eScreen(eScreen *screen, WINDOW_TYPE type);
void insert_char_infile_eScreen(eScreen *screen, int input);
#endif
