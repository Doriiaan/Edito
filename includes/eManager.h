#ifndef __EMANAGER_H__
#define __EMANAGER_H__

#include "eFile.h"
#include "eScreen.h"
#include "eLine.h"

typedef enum {
	NORMAL,
	WRITE	

} MODE;

typedef struct {

	/* Editor mode */
	MODE mode;

	/* View */
	eScreen *screen;

	/* Model */
	//eBar *bar;

	//eMenu *menu;

	eFile *file;

	/* Current line in the file */
	eLine *line;

} eManager;


eManager *create_eManager();
void delete_eManager(eManager **manager);

void set_eScreen_eManager(eManager *manager, eScreen *screen);
//void set_menu_eManager(eManager *manager, eMenu *menu);
//void set_bar_eManager(eManager *manager, eBar *menu);
void set_eFile_eManager(eManager *manager, eFile *file);
bool run_eManager(eManager *manager);

#endif
