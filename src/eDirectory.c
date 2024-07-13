#include "eDirectory.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <libgen.h> /* basename, dirname */
#include <dirent.h> /* opendir, readdir */
#include <unistd.h>

/**
 * @brief The dir_permissions() function return the permission of the directory designed by realpath.
 *
 * @param realpath: path (absolute of relative) of the directory 
 * @return PERM value defining the permission of the directory.
 */
PERM dir_permissions(const char *realpath)
{
	errno=0;
	if(access(realpath, X_OK|R_OK) == -1)
	{
		if(errno==ENOENT)
		{
			return p_CREATE;
		}
		else
		{
			return p_NOPERM;
		}
	}

	if(access(realpath, W_OK) == -1)
	{
		return p_READONLY;
	}
	else
	{
		return p_READWRITE;
	}
}


eDirectory *create_eDirectory(char *realpath)
{
	eDirectory *directory = NULL;
	PERM permissions; 
	DIR *dir = NULL;
    struct dirent *elem = NULL;
	struct stat elem_info;
	size_t alloc_size = 0;

	permissions = dir_permissions(realpath);
	if(permissions == p_NOPERM)
		return NULL;

	if(permissions == p_CREATE)
	{
		if(mkdir(directory->realpath, 755) == -1)
			return NULL;
	}

	directory = (eDirectory *) malloc(sizeof(eDirectory));
	if(directory == NULL)
		return NULL;

	directory->permissions = permissions;	
	directory->realpath = strdup(realpath);
	directory->dirname = basename(realpath);
	directory->path = dirname(realpath);
	directory->n_files = 0;
	directory->alloc_files_size = 0;
	directory->n_dirs = 0;
	directory->alloc_dirs_size = 0;
	directory->files = NULL;
	directory->dirs = NULL;

	dir = opendir(directory->realpath);
	if(dir == NULL)
	{
		return NULL;
	}

	while((elem = readdir(dir)) != NULL)
	{
		if(!strcmp(elem->d_name, ".") || !strcmp(elem->d_name, ".."))
			continue;

		stat(elem->d_name, &elem_info);

		if(S_ISREG(elem_info.st_mode))
		{
			if(directory->n_files >= directory->alloc_files_size)
			{
				alloc_size = get_next_power_of_two(directory->n_files+1);
				directory->files = realloc(directory->files, sizeof(eFile *)*alloc_size);
				if(directory->files == NULL)
				{
					delete_eDirectory(&directory);
					return NULL;
				}
				directory->alloc_files_size = alloc_size;
				alloc_size = 0;
			}
			directory->files[directory->n_files] = create_eFile(elem->d_name);
			directory->n_files++;
		}
		else if(S_ISDIR(elem_info.st_mode))
		{
			if(directory->n_dirs >= directory->alloc_dirs_size)
			{
				alloc_size = get_next_power_of_two(directory->n_dirs+1);
				directory->dirs = realloc(directory->dirs, sizeof(eDirectory *)*alloc_size);
				if(directory->dirs == NULL)
				{
					delete_eDirectory(&directory);
					return NULL;
				}
				directory->alloc_dirs_size = alloc_size;
				alloc_size = 0;
			}
			directory->dirs[directory->n_dirs] = create_eDirectory(elem->d_name);
			directory->n_dirs++;
		}
	}
	closedir(dir);

	return directory;	
}


void delete_eDirectory(eDirectory **directory)
{
	unsigned int i = 0;

	if(*directory == NULL)
		return;

	for(i=0; i<(*directory)->n_dirs; i++)
		delete_eDirectory(&((*directory)->dirs[i]));

	for(i=0; i<(*directory)->n_files; i++)
		delete_eFile(&((*directory)->files[i]));
	
	free((*directory)->realpath);
	free(*directory);
}
