#ifndef __UTIL_H__
#define __UTIL_H__

/**
 * @enum File and eRepository permissions enumeration
 */
typedef enum
{
	p_NOPERM,
	p_READONLY,
	p_READWRITE,
	p_CREATE
} PERM;


unsigned int get_next_power_of_two(unsigned int n);


/*
 * @brief The digit_number() function return the number of digit in a number.
 */
int digit_number(unsigned int n);

#endif
