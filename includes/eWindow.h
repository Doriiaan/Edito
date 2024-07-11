/**
 * @file 		eWindow.h
 * @brief 		eWindow Header
 * @author 		ALARY Dorian
 * @version 	0.1
 * @date 		23/06/2024
 * @copyright 	GNU Public License.
 *
 */

#ifndef __EWINDOW_H__
#define __EWINDOW_H__

#define WINDOWS_NUMBER 7

#include <ncurses.h>

/**
 * @enum Window type enumeration
 */
typedef enum {

	REPOSITORY=0,
	BAR_ITEMS=1,
	BAR_BOX=2,
	FILE_BOX=3,
	FILE_LINESNUMBER=4,
	FILE_CONTENT=5,
	POPUP=6

} WINDOW_TYPE;

/**
 * @struct Window structure to keep all information about a window 
 */
typedef struct eWindow {

	unsigned int width;

	unsigned int height;

	unsigned int x;

	unsigned int y;

	WINDOW *window;

	struct eWindow *orig;

} eWindow;


/**
 * @brief The create_eWindow() function allocate and initialize an eWindow structure.
 *
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note delete_eWindow() must be called before exiting.
 */
eWindow *create_eWindow(size_t height, size_t width, unsigned int y, unsigned int x);


/**
 * @brief The create_der_eWindow() function allocate and initialize an eWindow structure.
 *
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note y and x are relative to the origin of the eWindow orig.
 * @note delete_eWindow() must be called before exiting.
 */
eWindow *create_der_eWindow(eWindow *orig, size_t height, size_t width, unsigned int y, unsigned int x);


/**
 * @brief The delete_eWindow() function deallocate the eWindow structure and set the pointer to the structure to NULL.
 *
 * @param window eWindow pointer pointer
 */
void delete_eWindow(eWindow **window);

#endif
