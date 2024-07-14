#include "eDirectory.h"
#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
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
	char *elem_real_path = NULL;
	size_t elem_real_path_length = 0;

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

	if(directory->realpath[strlen(directory->realpath)-1] == '/')	
		directory->realpath[strlen(directory->realpath)-1] = 0;
	
	directory->dirname = ((directory->dirname = strrchr(directory->realpath, '/')) != NULL) ? directory->dirname+1 : directory->realpath;
	
	directory->n_files = 0;
	directory->alloc_files_size = 0;
	directory->n_dirs = 0;
	directory->alloc_dirs_size = 0;
	directory->files = NULL;
	directory->dirs = NULL;
	directory->is_open = false;

	dir = opendir(directory->realpath);
	if(dir == NULL)
	{
		return NULL;
	}

	while((elem = readdir(dir)) != NULL)
	{
		if(!strcmp(elem->d_name, ".") || !strcmp(elem->d_name, ".."))
			continue;

		if(elem_real_path_length < sizeof(char)*(strlen(directory->realpath)+strlen(elem->d_name)+2))
		{	
			elem_real_path_length = sizeof(char)*(strlen(directory->realpath)+strlen(elem->d_name)+2);
			elem_real_path = realloc(elem_real_path, elem_real_path_length);
			memset(elem_real_path, 0, elem_real_path_length);
		}
		
		strcpy(elem_real_path, directory->realpath);
		strcat(elem_real_path, "/");
		strcat(elem_real_path, elem->d_name);

		if(stat(elem_real_path, &elem_info) != 0)
			continue;


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
			directory->files[directory->n_files] = create_eFile(elem_real_path);
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
			directory->dirs[directory->n_dirs] = create_eDirectory(elem_real_path);
			directory->n_dirs++;
		}
	}
	free(elem_real_path);
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
