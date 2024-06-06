#ifndef __EFILE_H__
#define __EFILE_H__

#include "eLine.h"

typedef struct efile_s
{
	FILE *fp;
	eLine *elines;
	int n_elines;
} eFile;

eFile* open_eFile(const char *filename, const char *mode);

#endif
