#ifndef __UTIL_H__
#define __UTIL_H__

/**
 * @enum File permissions enumeration
 */
typedef enum
{
	p_NOPERM,
	p_READONLY,
	p_READWRITE,
	p_CREATE
} PERM;


unsigned int get_next_power_of_two(unsigned int n);

#endif
