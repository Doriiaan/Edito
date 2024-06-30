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

#define WINDOWS_NUMBER 6 

/**
 * @enum Window type enumeration
 */
typedef enum {

	MENU=0,
	BAR=1,
	FILE_BOX=2,
	FILE_LINESNUMBER=3,
	FILE_CONTENT=4,
	POPUP=5

} WINDOW_TYPE;

/**
 * @struct Window structure to reprensent keep all information about a window 
 */
typedef struct {

	unsigned int width;

	unsigned int height;

	unsigned int x;

	unsigned int y;

	unsigned int x_cursor;

	unsigned int y_cursor;

	WINDOW *window;

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
 * @brief The delete_eWindow() function deallocate the eWindow structure and set the pointer to the structure to NULL.
 *
 * @param window eWindow pointer pointer
 */
void delete_eWindow(eWindow **window);

#endif
