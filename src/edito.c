/**
 * ===================================================
 * @file edito.h
 * @brief Contain the main function of edito
 * @author ALARY Dorian
 * @version 0.1
 * @date 23/06/2024
 * @copyright GNU Public License
 * ===================================================
 */

#include <stddef.h>
#include <stdlib.h> 
#include <locale.h>
#include <stdbool.h>

#include "eScreen.h"
#include "eFile.h"
#include "eManager.h"

void init_terminal(void);
void reset_terminal(void);

int main()
{
	bool run = true;
	
	eManager *manager;
	eScreen *screen;
	eFile *file;

	/* Terminal initialization */
	init_terminal();	

	/* Screen structure initialization */
	if((screen = create_eScreen(LINES, COLS)) == NULL)
	{
		reset_terminal();
		exit(EXIT_FAILURE);
	}

	/* File structure initialization */
	if((file = create_eFile("text.txt")) == NULL)
	{
		reset_terminal();
		exit(EXIT_FAILURE);
	}

	/* Manager structure initialization */
	if((manager = create_eManager()) == NULL)
	{
		reset_terminal();
		exit(EXIT_FAILURE);
	}
	set_eScreen_eManager(manager, screen);
	set_eFile_eManager(manager, file);

	/* Print every Windows */
	update_all_eScreen(screen);

	/* Main loop */
	while(run)
	{
		run = run_eManager(manager);
	}

	delete_eScreen(&screen);
	delete_eFile(&file);
	delete_eManager(&manager);

	reset_terminal();
	
	return 0;
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
	refresh();
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
