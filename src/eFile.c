#include <stdio.h> /* printf, FILE */
#include <stdlib.h> /* malloc */
#include <string.h> /* strlen, strncpy, strncat */
#include <sys/stat.h> /* star structure */
#include <errno.h> /* errno code */

#include "eFile.h"
#include "eLine.h"

#define BUFFER_LENGTH 256 


/* return FILE pointer or NULL, and trace error */
FILE *open_fd(const char *filename)
{
	int fd; /* file desciptor returned by open */
	FILE *file=NULL; /* FILE pointer returned by fdopen */
	struct stat filestat; /* stat structure to test information about the file "filename" */
	int uid, gid; /* uid and gid of process */
	int flags=0;


	if(stat(&filestat, filename) == -1)
	{
		/* If the file does not exist, create it */
		if(errno == ENOENT) 
		{
			flags |= O_CREAT;
		}
		else 
		{
			/*TODO: TRACE */
			return NULL;
		}
	}
	

	/* Regular file process */
	if(!IS_REG(filestat.st_mode))
	{
		/*TODO:TRACE*/
		return NULL;
	}
	

	uid = getuid();
	gid = getgid();

	/* If user is the proprietary */
	if(uid == filestat.st_uid)
	{
		/* Do user has read right ? */
		if(filestat.st_mode & S_IRUSR)
		{
			/*TODO: TRACE */
			return NULL;
		}

		/* Do user has write right ? */
		if(filestat.st_mode & S_IWUSR)
		{
			flags |= O_RDWR;
		}
		else
		{
			flags |= O_RDONLY;
		}
	}
	/* Else if user is in the group */
	else if(gid == filestat.st_gid)
	{	
		/* Do group has read right ? */
		if(filestat.st_mode & S_IRGRP)
		{
			/*TODO: TRACE */
			return NULL;
		}

		/* Do group has write right ? */
		if(filestat.st_mode & S_IWGRP)
		{
			flags |= O_RDWR;
		}
		else
		{
			flags |= O_RDONLY;
		}
	}
	/* Else, process as other */
	else
	{
		/* Do other has read right ? */
		if(filestat.st_mode & S_IROTH)
		{
			/*TODO: TRACE */
			return NULL;
		}

		/* Do other has write right ? */
		if(filestat.st_mode & S_IWOTH)
		{
			flags |= O_RDWR;
		}
		else
		{
			flags |= O_RDONLY;
		}
	}
}


eFile* open_eFile(const char *filename)
{
	FILE *fp=NULL;
	eFile *efile=NULL;
	eLine *current=NULL, *previous=NULL;
	char buffer[BUFFER_LENGTH];
	int pos = 1;


	/* Open file */
	if((fp = fopen(filename, mode)) == NULL)
	{
		/* TODO: TRACE */
		return NULL;
	}


	/* Allocate and fill the data structure */
	efile = (eFile *) malloc(sizeof(eFile));
	efile->fp = fp;


	/* Loop to read lines of the file*/
	while(fgets(buffer, BUFFER_LENGTH, efile->fp) != NULL)
	{
		current = create_eLine(buffer, BUFFER_LENGTH, pos, previous, NULL);
		if(current == NULL)
		{
			/* TODO: TRACE + close eFile */
			return NULL;
		}

		/* If it is not the end of line */
		while(buffer[strlen(buffer)-1] != '\n' && fgets(buffer, BUFFER_LENGTH, efile->fp) != NULL)
		{
			if(insert_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				/* TODO: TRACE */
				break;
			}
		}

		/* If the last line does not have a \n */
		if(get_char_eLine(current, get_length_eLine(current)-1)!= '\n')
			if(insert_eLine(current, buffer, BUFFER_LENGTH, current->length))
			{
				/* TODO: TRACE */
			}

	   	if(pos==1)
			efile->first = current;


		if(get_previous_eLine(current) != NULL)
			set_next_eLine(get_previous_eLine(current), current);


		/* reinit for future lines */
		pos++;
		previous = current;
		current = NULL;
	}

	efile->n_elines = pos-1;

	return efile;
}

int write_eFile(eFile *ef);
int close_eFile(eFile **ef)
{
	int result;
	eLine *current = (*ef)->first, *temp=NULL;

	if((result = fclose((*ef)->fp)))
		return result;

	while(current->next)
	{
		temp = current->next;
		delete_eLine(&current);
		current = temp;
	}

	free(*ef);
	*ef = NULL;
	return 0;
}
