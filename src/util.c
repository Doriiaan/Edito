#include "util.h"

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
	echo(); /* deactivate echo from getch */

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
	reset_terminal();
	fprintf(stderr, "ERROR (code %d) %s:%d --- %s\n", code, __FILE__, __LINE__, str);
	exit(code);
}
