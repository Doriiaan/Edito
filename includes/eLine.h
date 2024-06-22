#ifndef __ELINE_H__
#define __ELINE_H__

#include <stddef.h> /* size_t */

/* eLine structure */
typedef struct eLine
{
	/* characters of the line, 4096 character maximum including \n character */
	char *string; 
	
	/* length of line */
	size_t length;

	/* Allocated size */
	size_t alloc_size;	
	
	/* position of the line in the file */
	unsigned int pos;
	
	/* previous line of NULL */
	struct eLine *previous; 
	
	/* next line or NULL*/
	struct eLine *next; 
} eLine;


/* GETTERs AND SETTERs */
size_t get_length_eLine(const eLine *eline);
unsigned int get_pos_eLine(const eLine *eline);
int get_char_eLine(const eLine *eline, size_t n);
eLine *get_next_eLine(const eLine *eline);
eLine *get_previous_eLine(const eLine *eline);

void set_next_eLine(eLine *eline, eLine *next);
void set_previous_eLine(eLine *eline, eLine *previous);


/* FUNCTIONs */
eLine *create_eLine(char *string, size_t length, unsigned int pos, eLine *previous, eLine *next);
void delete_eLine(eLine **eline);
int insert_eLine(eLine *eline, const char *string, size_t length, unsigned int pos);
#endif
