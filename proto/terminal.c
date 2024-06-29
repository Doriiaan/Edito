#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

#include <ncurses.h>
#include <panel.h>

#include "eFile.h"


typedef struct {
	int n_lines;
	int n_cols;
	eLine *first;
	eLine *end;
} eScreen;


eScreen * create_eScreen(eLine *first, int n_lines, int n_cols);
void print_code(WINDOW *codefile_win, WINDOW *codenumbers_win, eScreen *screen);
void reset_terminal(void);
void init_terminal(void);
void error(int code, char *str);


int main(void)
{
	eFile *file=NULL;
	eScreen *screen=NULL;

	int lines_menu, cols_menu;
	int lines_files, cols_files;
	int lines_code_box, cols_code_box;	
	int lines_code_numbers, cols_code_numbers;	
	int lines_code_file, cols_code_file;	
	
	WINDOW *w_menu=NULL, *w_files=NULL, *w_code_box=NULL, *w_code_numbers=NULL, *w_code_file=NULL;
	
	int ch;
	bool run = true;

	if((file = create_eFile("file.txt")) == NULL)
	{
		return 1;
	}

	
	init_terminal();

	lines_menu=LINES, cols_menu=COLS/5;
	lines_files=LINES/10, cols_files=(COLS*4)/5;
	lines_code_box=(LINES*9)/10, cols_code_box=(COLS*4)/5;	
	lines_code_numbers=lines_code_box-2, cols_code_numbers=5;	
	lines_code_file=lines_code_box-2, cols_code_file=cols_code_box-cols_code_numbers-2;

	screen = create_eScreen(file->first, lines_code_file, cols_code_file);

	w_menu = newwin(lines_menu, cols_menu, 0, 0);
	w_files = newwin(lines_files, cols_files, 0, COLS/5);
	w_code_box = newwin(lines_code_box, cols_code_box, LINES/10, COLS/5);
	w_code_numbers = newwin(lines_code_numbers, cols_code_numbers, LINES/10+1, COLS/5+1);
	w_code_file = newwin(lines_code_file, cols_code_file, LINES/10+1, COLS/5+cols_code_numbers+1);

	box(w_menu, 0, 0);
	box(w_files, 0, 0);
	box(w_code_box, 0, 0);

	refresh();
	wrefresh(w_menu);
	wrefresh(w_files);
	wrefresh(w_code_box);
	print_code(w_code_file, w_code_numbers, screen);

	while(run)
	{
		ch = getch();
		switch(ch)
		{
			case 'q':
				run = false;
				break;

			case KEY_DOWN:
				if(screen->end->next)
				{
					screen->first = screen->first->next;
					screen->end = screen->end->next;
					print_code(w_code_file, w_code_numbers, screen);
				}
				break;

			case KEY_UP:
				if(screen->first->previous)
				{
					screen->first = screen->first->previous;
					screen->end = screen->end->previous;
					print_code(w_code_file, w_code_numbers, screen);
				}
				break;
		}
	}	


	delwin(w_menu);
	delwin(w_files);
	delwin(w_code_box);
	delwin(w_code_numbers);
	delwin(w_code_file);

	
	delete_eFile(&file);
	free(screen);

	reset_terminal();
	return 0;
}


void print_code(WINDOW *codefile_win, WINDOW *codenumbers_win, eScreen *screen)
{
	eLine *current_line = screen->first;

	int pos=0;
	int line_number=screen->first->pos;
	wborder(codenumbers_win, ' ', 0, ' ', ' ', ' ', ACS_VLINE, ' ', ACS_VLINE);
	while(pos < screen->n_lines)
	{
		if(current_line)
		{
			mvwprintw(codenumbers_win, pos, 0, "%3d", line_number);
			for(size_t i_part_of_line=0 ; i_part_of_line < current_line->length ; i_part_of_line+=screen->n_cols)
			{
				
				mvwprintw(codefile_win, pos, 0, "%.*s", screen->n_cols, current_line->string+i_part_of_line);
				pos++;
			}
			
			current_line = current_line->next;
		}
		else
		{
			mvwprintw(codenumbers_win, pos, 0, "%3c", '~');
			pos++;
		}
		line_number++;
	}
	wnoutrefresh(codefile_win);
	wnoutrefresh(codenumbers_win);
	doupdate();
}


eScreen * create_eScreen(eLine *first, int n_lines, int n_cols)
{
	eLine *current=NULL;

	eScreen *screen = (eScreen *) malloc(sizeof(eScreen));
	screen->n_lines = n_lines;
	screen->n_cols = n_cols;
	screen->first = first;
	screen->end = first;

	current=first;
	for(int i=0 ; i<n_lines ; i++)
	{
		if(current)
		{
			screen->end=current;
			current=current->next;
		}
	}

	return screen;
}


/*
 * @brief Initialize the terminal for the applciation
 *
 */
void init_terminal(void)
{
	setlocale(LC_ALL, "");

    initscr(); /* Init window structure */

	noecho(); /* deactivate echo from getch */
	raw(); /* characters typed are immediately passed through to the user program and signal are uninterpreted */
	keypad(stdscr, TRUE); /* activate KEY_UP, KEY_RIGHT, ... */
}


/*
 * @brief Reset the terminal to quit the application
 *
 *
 */
void reset_terminal(void)
{
	endwin(); /* Restore terminal */
}



/* 
 * @brief Trace error on stderr
 *
 * @param (IN)	int code: errcode
 * @param (IN)	char *str: error message to print
 *
 * @note Perform all the action to correctly end ncurses program
 *
 */
void error(int code, char *str)
{
	endwin();
	fprintf(stderr, "ERROR (code %d) %s:%d --- %s\n", code, __FILE__, __LINE__, str);
	exit(code);
}
