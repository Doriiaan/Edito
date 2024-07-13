#ifndef __EDIRECTORY_H__
#define __EDIRECTORY_H__

#include "eFile.h"
#include "util.h"

typedef struct eDirectory {

	unsigned int n_files;
	
	unsigned int n_dirs;

	PERM permissions;

	eFile **files;

	size_t alloc_files_size;
	
	struct eDirectory **dirs;
	
	size_t alloc_dirs_size;
	
	char *dirname;

	char *path;

	char *realpath;

} eDirectory;


eDirectory *create_eDirectory(char *realpath);

void delete_eDirectory(eDirectory **directory);

#endif
