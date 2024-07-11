#ifndef __EBAR_H__
#define __EBAR_H__

#include "eFile.h"

typedef struct {

	/** File count */
	unsigned int n_files;
	
	/** List of eFile */
	eFile **files;

	/** List allocation size */
	size_t alloc_size;

} eBar;


eBar *create_eBar();

void delete_eBar(eBar **bar);

int add_file_eBar(eBar *bar, eFile *file);

int remove_file_eBar(eBar *bar, unsigned int index);

eFile *get_file_eBar(eBar *bar, unsigned int index);

#endif
