# Architecture

Edito uses an MVC pattern.

A file corresponds to a data structure and its algorithms.

## Model

_Components: eDirectory, eFile, eLine, eBar_

### eDirectory

eDirectory structure contains all the information about a directory and its children. This information includes:
- Its relative path and name.
- Its permissions.
- The list of child eDirectories.
- The number of eDirectories in the list.
- The list of child eFiles.
- The number of eFile in the list.
- Boolean indicating whether the folder is open or not.

It is possible to get the item in the ith place in the eDirectory and its children. The third item is searched in depth in the open eDirectory.

### eFile

eFile structure contains all the information about a file. This information includes:
- Its relative path and name.
- Its permissions.
- The linked list of its lines. See eLine for the structure of a node.
- The number of line in the list.
- The first file line, the current file line and the first screen line.
- Boolean indicating whether the file is saved or not.

It is possible to open or close an eFile. It is also possible to write the eFile on the disk. Finally, it is possible to add an empty line, delete a line, add a string or a character to a line, or delete a string or a character from a line.

### eLine

eLine structure contains all the information about a Line. This information includes:
- Its string.
- Its length.
- The previous line.
- The next line.
- Its line number.

It is possible to add, delete or get strings or characters from the eLine.

### eBar

eBar structure contains eFiles that are open.

It is possible to add, delete or get files from eBar.

## Vue

_Components: eScreen, eWindow, eMenu_

### eScreen

eScreen is the controller for items displayed on the screen. eScreen structure includes:
- The width and height of the screen.
- The list of eWindow.
- The list of eMenu.

It is possible to :
- update certain parts of the screen, such as the directory, the bar or the current file.
- get user input.
- perform many actions on the windows and menus. See incldues/eScreen.h for more details.

### eWindow

eWindow structure contains ncurses window and other information such as:
- The width and height of the window.
- x and y position of the window.
- The parent eWindow, if any.

eWindow file also declares an enumeration of the following window types:
- WDIR\_BOX -> Box of the directory
- WDIR\_ITEMS -> Items of the directory
- WBAR\_BOX -> Box of the bar
- WBAR\_ITEMS -> Items of the bar
- WFILE\_BOX -> Box of the file
- WFILE\_LNUM -> Line numbers of the file
- WFILE\_CNT -> Content of the file
- WHELP -> Help

### eMenu

eMenu structure contains ncurses menu and other information such as :
- ncurses parent window (box).
- ncurses sub window (items).
- Virtual items title.
- Physical items title.
- Number of rows.
- Number of columns.
- Number of items.
- Number of scroll.
- Boolean indicating whether the menu is columnar or not.

Virtual item title and Physical items title allow menus to emulate the same behaviour as windows (virtual and physical screen), this allows dynamic menus to be created (which do not exist in ncurses).

The workflow is to add or remove items from the menu, which will modify the virtual menu. Then refresh the menu to copy the contents of the virtual menu into the physical menu.

## Controler

_Components: edito(main), eManager_

### edito(main)

Main program.

### eManager

eManager structure contains all information to manage the main algorithm:
- eScreen.
- eBar.
- Root eDirectory.
- Current eFile.
- The mode (WRITE, DIR or BAR) and last mode.
- Next help message if any.

The main function is run\_eManager(). This function receives data from the user, processes it ( changes the model and the view) and updates the screen.
