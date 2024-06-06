#include <ncurses.h>
#include <stdlib.h>

void reset_terminal(void);
void init_terminal(void);
void error(int code, char *str);

int main(void)
{
	int x, y;
	int c;

	init_terminal();

	/* black screen */
	attron(A_REVERSE);
	
	for(y=0 ; y<(LINES) ; y++)
		for(x=0 ; x<(COLS) ; x++)
			mvaddch(y, x, ' '); /* do not catch error, because print character on the last columns return an errori */

	attroff(A_REVERSE);

	/* refresh window */
    refresh();

	y=LINES/2;
	x=COLS/2;
	move(y, x);
	addch(' ');
	
	while((c=getch())!='q')
	{
		switch(c)
		{
			case KEY_UP:
				if(y>0)
				{
					y--;
					move(y, x);
					addch(' ');
				}
				break;
			case KEY_DOWN:
				if(y<((LINES)-1))
				{
					y++;
					move(y, x);
					addch(' ');
				}
				break;
			case KEY_RIGHT:
				if(x<((COLS)-1))
				{
					x++;
					move(y, x);
					addch(' ');
				}
				break;
			case KEY_LEFT:
				if(x>0)
				{
					x--;
					move(y, x);
					addch(' ');
				}
				break;
			default:
				break;
		}
		refresh();
	}

	reset_terminal();

    return 0;
}


/*
 * @brief Initialize the terminal for the applciation
 *
 */
void init_terminal(void)
{
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
