/**
 * ===================================================
 * @file edito.h
 * @brief Contain the main function of edito
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License
 * ===================================================
 */

#include "eScreen.h"
#include "eFile.h"
#include "eManager.h"

#include <stdlib.h> 
#include <stdbool.h>
#include <locale.h>


void init_terminal(void);
void reset_terminal(void);
void usage(void);

int main(int argc, char *argv[])
{
	bool run = true;
	
	eManager *manager = NULL;
	eScreen *screen = NULL;
	eBar *bar = NULL;
	eDirectory *project_repo = NULL;
	char *reponame = 0; 

	if(argc == 1)
	{
		reponame = ".";
	}
	else if(argc == 2)
	{
		reponame = argv[1];
	}
	else
	{
		usage();
		exit(EXIT_FAILURE);
	}
		

	/* Terminal initialization */
	init_terminal();	

	/* Screen structure initialization */
	if((screen = create_eScreen(LINES, COLS)) == NULL)
	{
		reset_terminal();
		exit(EXIT_FAILURE);
	}
	
	/* Bar structure creation */
	if((bar = create_eBar()) == NULL)
	{
		reset_terminal();
		exit(EXIT_FAILURE);
	}

	/* Repository structure creation */
	if((project_repo = create_eDirectory(reponame)) == NULL)
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
	set_eBar_eManager(manager, bar);
	set_eDirectory_eManager(manager, project_repo);

	manager->directory->is_open = true;

	fill_directory_menu_eManager(manager, manager->directory, 0);
	refresh_menu_eScreen(manager->screen, MDIR);

	send_help_msg_to_screen_eManager(manager);

	/* Print every Windows and Menus on the screen */
	update_all_eScreen(screen);
	
	/* Set cursor on the current menu item */
	move_current_item_menu_eScreen(manager->screen, MDIR);

	/* Main loop */
	while(run)
	{
		run = run_eManager(manager);
	}

	delete_eScreen(&screen);
	delete_eBar(&bar);
	delete_eDirectory(&project_repo);
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

	noecho(); /* Deactivate echo from getch */
	raw(); /* Deactivate buffering and disallow signals like Ctrl+C, Ctrl+S */
	set_escdelay(50);
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


void usage(void)
{
	printf("edito [directory]");
}
