/*
 * @brief The get_next_power_of_two() function is an intern function to calculate the next power of two after a number n.
 *
 * @example get_next_power_of_two(7) -> 8
 * @example get_next_power_of_two(16) -> 32
 * @example get_next_power_of_two(0) -> 1
 */
unsigned int get_next_power_of_two(unsigned int n)
{
	return (1 << (sizeof(unsigned int)*8 - __builtin_clz(n)));
}


/*
 * @brief The digit_number() function return the number of digit in a number.
 */
int digit_number(unsigned int n) 
{
    if (n < 10) 
		return 1;
    if (n < 100) 
		return 2;
    if (n < 1000) 
		return 3;
    if (n < 10000) 
		return 4;
    if (n < 100000) 
		return 5;
    if (n < 1000000) 
		return 6;
    if (n < 1000000) 
		return 6;
    if (n < 10000000) 
		return 7;
    if (n < 100000000) 
		return 8;
    if (n < 1000000000) 
		return 9;

    return 10;
}
