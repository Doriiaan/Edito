/**
 * @file util.h
 * @brief util functions header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 */


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


/*
 * @brief The get_next_power_of_two() function is an intern function to
 *        calculate the next power of two after a number n.
 *
 * @example get_next_power_of_two(7) -> 8
 * @example get_next_power_of_two(16) -> 32
 * @example get_next_power_of_two(0) -> 1
 */
unsigned int get_next_power_of_two(unsigned int n);


/*
 * @brief The digit_number() function return the number of digit in a number.
 */
int digit_number(unsigned int n);

#endif
