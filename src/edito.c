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
	eFile *file = NULL;
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
	fill_dir_eManager(manager, manager->directory, 0);
	refresh_menu_eScreen(manager->screen, MDIR);

	/* Print every Windows */
	update_all_eScreen(screen);

	add_item_menu_eScreen(manager->screen, MBAR, "lala");
	add_item_menu_eScreen(manager->screen, MBAR, "lolo");
	add_item_menu_eScreen(manager->screen, MBAR, "lulu");

	refresh_menu_eScreen(manager->screen, MBAR);

	add_item_menu_eScreen(manager->screen, MBAR, "iaia");
	add_item_menu_eScreen(manager->screen, MBAR, "hhqsdbfq");
	add_item_menu_eScreen(manager->screen, MBAR, "qsdfsqdf");
	add_item_menu_eScreen(manager->screen, MBAR, "loulou");

	refresh_menu_eScreen(manager->screen, MBAR);
	
	update_all_eScreen(manager->screen);

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

	noecho(); /* Deactivate echo from getch */
	cbreak(); /* Deactivate buffering but allow Ctrl+C */
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
