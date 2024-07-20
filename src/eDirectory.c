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


/**
 * @brief The create_eDirectory() function allocate and initialize an eDirectory and is child directories andd files.
 *
 * @param realpath: Path + '/' + name of the directory
 *
 * @return Pointer on the eDirectory structure or NULL if allocation failed.
 *
 * @note delete_eDirectory() must be called before exiting.
 */
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
		if(mkdir(realpath, 0x755) == -1)
			return NULL;
	}

	directory = (eDirectory *) malloc(sizeof(eDirectory));
	if(directory == NULL)
		return NULL;

	directory->permissions = permissions;	
	directory->realpath = strdup(realpath);


	/* Delete the '/' at the end */
	if(directory->realpath[strlen(directory->realpath)-1] == '/')	
		directory->realpath[strlen(directory->realpath)-1] = 0;
	
	/* Dirname is equal to pointer on the last occurence of '/'+1 (last part) or realpath if there is no '/' */ 
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

	/* For each elem of physical directory */
	while((elem = readdir(dir)) != NULL)
	{
		if(!strcmp(elem->d_name, ".") || !strcmp(elem->d_name, ".."))
			continue;

		/* Allocate string to contain path of elem */
		if(elem_real_path_length < sizeof(char)*(strlen(directory->realpath)+strlen(elem->d_name)+2))
		{	
			/* Path + '/' + Name + 0 */
			elem_real_path_length = sizeof(char)*(strlen(directory->realpath)+strlen(elem->d_name)+2);
			elem_real_path = realloc(elem_real_path, elem_real_path_length);
			memset(elem_real_path, 0, elem_real_path_length);
		}
		
		strcpy(elem_real_path, directory->realpath);
		strcat(elem_real_path, "/");
		strcat(elem_real_path, elem->d_name);

		/* Get info of elem */
		if(stat(elem_real_path, &elem_info) != 0)
			continue;


		/* If elem is a regular file */
		if(S_ISREG(elem_info.st_mode))
		{
			/* Allocate memory to store files */
			if(directory->n_files+1 > directory->alloc_files_size)
			{
				alloc_size = get_next_power_of_two(directory->n_files+1);
				directory->files = (eFile **) realloc(directory->files, sizeof(eFile *)*alloc_size);
				if(directory->files == NULL)
				{
					continue;
				}
				directory->alloc_files_size = alloc_size;
				alloc_size = 0;
			}
			directory->files[directory->n_files] = create_eFile(elem_real_path);
			directory->n_files++;
		}
		/* If elem is a directory */	
		else if(S_ISDIR(elem_info.st_mode))
		{
			/* Allocate memory to store dirs */
			if(directory->n_dirs+1 > directory->alloc_dirs_size)
			{
				alloc_size = get_next_power_of_two(directory->n_dirs+1);
				directory->dirs = (eDirectory **) realloc(directory->dirs, sizeof(eDirectory *)*alloc_size);
				if(directory->dirs == NULL)
				{
					continue;
				}
				directory->alloc_dirs_size = alloc_size;
				alloc_size = 0;
			}
			/* Recursive */
			directory->dirs[directory->n_dirs] = create_eDirectory(elem_real_path);
			directory->n_dirs++;
		}
	}
	free(elem_real_path);
	closedir(dir);

	return directory;	
}


/**
 * @brief The delete_eDirectory() function delete and deallocate eDirectory and set pointer to NULL.
 *
 * @param directory: eDirectory pointer pointer
 */
void delete_eDirectory(eDirectory **directory)
{
	unsigned int i = 0;

	if(*directory == NULL)
		return;

	for(i=0; i<(*directory)->n_dirs; i++)
		delete_eDirectory(&(*directory)->dirs[i]);

	for(i=0; i<(*directory)->n_files; i++)
		delete_eFile(&(*directory)->files[i]);
	
	free((*directory)->dirs);
	free((*directory)->files);
	free((*directory)->realpath);
	free(*directory);
}


/** 
 * @brief The get_item_at_index_eDirectory() function returns either a directory or a file at the index. Search is recursive.
 *
 * @param directory: eDirectory pointer
 * @param item_index: item of pointer to be returned
 * @param out_directory: eDirectory pointer returned
 * @param out_file: eFile pointer returned
 *
 * @return 0 on success or -1 in failure or a positive number indicating the index overflow in comparison with the number of files/folders.
 */
int get_item_at_index_eDirectory(eDirectory *directory, unsigned int item_index, eDirectory **out_directory, eFile **out_file)
{
	int result = 0;

	if(directory == NULL)
		return -1;

	if(item_index == 0)
	{
		*out_directory = directory;
		return 0;
	}

	for(unsigned int i=0; i<directory->n_dirs; i++)
	{
		item_index--;
		if(item_index == 0)
		{
			*out_directory = directory->dirs[i];
			return 0;
		}

		if(directory->dirs[i]->is_open)
		{
			result = get_item_at_index_eDirectory(directory->dirs[i], item_index, out_directory, out_file);
			if(result == -1)
				return -1;
			else if(result == 0)
				return 0;
			else
				item_index = result;
		}
	}

	for(unsigned int i=0; i<directory->n_files; i++)
	{
		item_index--;
		if(item_index == 0)
		{
			*out_file = directory->files[i];
			return 0;
		}
	}

	return item_index;
}

