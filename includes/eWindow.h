/**
 * @file eWindow.h
 * @brief eWindow Header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 */

#ifndef __EWINDOW_H__
#define __EWINDOW_H__

#define WINDOWS_NUMBER 8

#include <ncurses.h>


/**
 * @enum Window type enumeration
 */
typedef enum {

    WDIR_BOX=0,
    WDIR_ITEMS,
    WBAR_BOX,
    WBAR_ITEMS,
    WFILE_BOX,
    WFILE_LNUM,
    WFILE_CNT,
    WHELP

} WINDOW_TYPE;


/**
 * @struct Window structure to keep all information about a window
 */
typedef struct eWindow {

    unsigned int width;

    unsigned int height;

    unsigned int x;

    unsigned int y;

    WINDOW * window;

    struct eWindow * orig;

} eWindow;


/**
 * @brief The create_eWindow() function allocate and initialize an eWindow
 *        structure.
 *
 * @param height: Window height (in lines)
 * @param width: Window width (in columns)
 * @param y: y position of the window in the Screen
 * @param x: x position of the winwow in the Screen
 *
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note delete_eWindow() must be called before exiting.
 */
eWindow * create_eWindow(size_t height,
                         size_t width,
                         unsigned int y,
                         unsigned int x);


/**
 * @brief The create_der_eWindow() function allocate and initialize an eWindow
 *        structure.
 *
 * @param orig: Window parent
 * @param height: Window height (in lines)
 * @param width: Window width (in columns)
 * @param y: y position of the window in the Screen
 * @param x: x position of the winwow in the Screen
 *
 * @return eWindow pointer or NULL if it was an error, see logs.
 *
 * @note y and x are relative to the origin of the eWindow orig.
 * @note delete_eWindow() must be called before exiting.
 */
eWindow * create_der_eWindow(eWindow * orig,
                             size_t height,
                             size_t width,
                             unsigned int y,
                             unsigned int x);


/**
 * @brief The delete_eWindow() function deallocate the eWindow structure and
 *        set the pointer to the structure to NULL.
 *
 * @param window eWindow pointer pointer
 */
void delete_eWindow(eWindow ** window);

#endif
