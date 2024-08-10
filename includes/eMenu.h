/**
 * @file eMenu.h
 * @brief eMenu Header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 */

#ifndef __EMENU_H__
#define __EMENU_H__

#include <ncurses.h>
#include <menu.h>

#define MENU_NUMBER 2


/**
 * @enum Menu type enumeration
 */
typedef enum {

    MDIR=0,
    MBAR

} MENU_TYPE;


/**
 * @struct Menu structure to keep all information about a menu
 */
typedef struct {

    /** Menu window */
    WINDOW * win;

    /** Menu sub window */
    WINDOW * sub;

    /** Menu */
    MENU * menu;

    /** Menu items */
    ITEM ** items;

    /** Menu virtual items title. Virtual items become physical items
        after calling refresh_eMenu() */
    char ** virtual_items_title;

    /** Menu physical items title */
    char ** physical_items_title;

    /** Number of rows of menu */
    int rows;

    /** Number of columns of menu */
    int columns;

    /** Number of items in virtual menu int because index curses parameter
        are integer */
    int n_items;

    /** Menu virtual items allocation size */
    int alloc_size;

    /** Number of scroll, useful after refresh the menu */
    int n_scroll;

    /** Do the menu displays items in column or in row.
        1 in column, 0 in row. */
    bool columnar;

} eMenu;


/**
 * @brief The create_eMenu() function allocate and initialize an eMenu
 *        structure.
 *
 * @param win: Menu window
 * @param sub: Menu sub window
 * @param columnar: true, if the menu displays items in columns, false if
 *                  it displays them in rows
 *
 * @return eMenu pointer or NULL if it was an error.
 *
 * @note delete_eMenu() must be called before exiting.
 */
eMenu * create_eMenu(WINDOW * win,
                     WINDOW * sub,
                     bool columnar);


/**
 * @brief The delete_eMenu() function deallocate the eMenu structure and
 *        set the pointer to the structure to NULL.
 *
 * @param menu: eMenu pointer pointer
 */
void delete_eMenu(eMenu ** menu);


/**
 * @brief The add_item_eMenu() add the item to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 * @param item: Item added
 *
 * @return 0 on success, -1 in failure.
 */
int add_item_eMenu(eMenu * menu,
                   char const * item);


/**
 * @brief The delete_item_eMenu() delete the item to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 * @param item: Item deleted
 *
 * @return 0 on success, -1 in failure.
 */
int delete_item_eMenu(eMenu * menu,
                      int index);


/**
 * @brief The erase_eMenu() erase the virtual menu. Useful to refresh the
 *        menu from a data structure.
 *
 * @param menu: eMenu pointer pointer
 */
void erase_eMenu(eMenu * menu);


/**
 * @brief The refresh_eMenu() set the physical menu to the virtual menu.
 *
 * @param menu: eMenu pointer pointer
 */
void refresh_eMenu(eMenu * menu);


/**
 * @brief The move_next_item_eMenu() move the cursor to the next item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_next_item_eMenu(eMenu * menu);


/**
 * @brief The move_previous_item_eMenu() move the cursor to the previous
 *        item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_previous_item_eMenu(eMenu * menu);


/**
 * @brief The move_current_item_eMenu(eMenu *menu) move the cursor to the
 *        current item.
 *
 * @param menu: eMenu pointer pointer
 */
void move_current_item_eMenu(eMenu * menu);


/**
 * @brief The move_pattern_item_eMenu() function move the cursor to the
 *        next item with pattern matching.
 *
 * @param menu: eMenu pointer pointer
 * @param pattern: pattern to match
 */
void move_pattern_item_eMenu(eMenu * menu,
                             char const * pattern);


/**
 * @bried The get_current_item_index_eMenu() function return the current
 *        item index.
 *
 * @param menu: eMenu pointer pointer
 *
 * @return The current item index.
 */
int get_current_item_index_eMenu(eMenu const * menu);


/**
 * @brief The set_cursor_position_eMenu() set the menu current position to
 *        the specified position.
 *
 * @param menu: eMenu pointer pointer
 * @param position: New current position
 */
void set_cursor_position_eMenu(eMenu * menu,
                               int position);

#endif
