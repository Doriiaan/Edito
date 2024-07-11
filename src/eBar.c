#include "eBar.h"
#include "eFile.h"
#include "util.h"

#include <stdlib.h>

eBar *create_eBar()
{
	eBar *bar = NULL;

	bar = (eBar *) malloc(sizeof(eBar));
	
	bar->files = NULL;
	bar->n_files = 0;
	bar->alloc_size = 0;

	return bar;
}


void delete_eBar(eBar **bar)
{
	if(*bar == NULL)
		return;

	if((*bar)->files)
		free((*bar)->files);

	free(*bar);
	*bar = NULL;
}


int add_file_eBar(eBar *bar, eFile *file)
{
	if(bar == NULL || file == NULL)
		return -1;

	if(bar->n_files >= bar->alloc_size)
	{
		bar->alloc_size = get_next_power_of_two(bar->n_files);
		bar->files = (eFile **) realloc(bar->files, bar->alloc_size*sizeof(eFile));
		if(bar->files == NULL)
		{
			return -1;
		}
	}

	if(open_eFile(file) == -1)
		return -1;

	bar->files[bar->n_files] = file;
	bar->n_files++;

	return 0;
}


eFile *get_file_eBar(eBar *bar, unsigned int index)
{
	if(bar == NULL || index >= bar->n_files)
		return NULL;

	return bar->files[index];
}


int remove_file_eBar(eBar *bar, unsigned int index)
{
	if(bar == NULL || index >= bar->n_files)
		return -1;

	bar->files[index] = NULL;
	bar->n_files--;
	return 0;
}
