#ifndef __EDIRECTORY_H__
#define __EDIRECTORY_H__

#include "eFile.h"
#include "util.h"

#include <stdbool.h>

typedef struct eDirectory {

	size_t alloc_files_size;
	
	size_t alloc_dirs_size;
	
	eFile **files;

	struct eDirectory **dirs;

	unsigned int n_files;
	
	unsigned int n_dirs;

	PERM permissions;
	
	char *dirname;

	char *realpath;

	bool is_open;

} eDirectory;


eDirectory *create_eDirectory(char *realpath);

void delete_eDirectory(eDirectory **directory);

int get_item_at_index_eDirectory(eDirectory *directory, unsigned int item_index, eDirectory **out_directory, eFile **out_file);
// ADD file
// ADD directory

// DEL file
// DEL directory

// GET files

// begin dir
// begin 

#endif
