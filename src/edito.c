/*
 * Edito main file
 *
 * ------------------------------------------------
 * Version : 0.0.0.0
 * Date : 01/06/2024
 * Brief : Creation of the file
 * -------------------------------------------------
 *
 *
 *
 */

#include <stdio.h>

#include "eLine.h" 


int main()
{
	char string[32] = "Hello world!";
	eLine *eline = create_eLine(string, 32, 0, NULL, NULL);
	
	insert_eLine(eline, "$baba$", 32, 6);
	
	delete_eLine(&eline);
	
	return 0;
}
