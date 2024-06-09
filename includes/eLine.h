#ifndef __ELINE_H__
#define __ELINE_H__

typedef struct eLine
{
	/* characters of the line, 4096 character maximum including \n character */
	char *string; 
	
	/* length of line */
	int length; 
	
	/* position of the line in the file */
	int pos;
	
	/* next line or NULL*/
	struct eLine *next; 
	
	/* previous line of NULL */
	struct eLine *previous; 
} eLine;

eLine *create_eLine(char *string, eLine *next, eLine *previous);

#endif
