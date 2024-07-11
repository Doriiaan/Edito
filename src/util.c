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
