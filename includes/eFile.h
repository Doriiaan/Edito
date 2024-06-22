#ifndef __EFILE_H__
#define __EFILE_H__

#include <stdio.h> /* FILE */

#include "eLine.h"

typedef struct efile_s
{
	FILE *fp;
	/* TODO add mode of file (append, read, write) */
	eLine *first;
	int n_elines;
} eFile;

eFile* open_eFile(const char *filename, const char *mode);
int close_eFile(eFile **ef);

#endif
