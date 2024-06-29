#ifndef __EWINDOW_H__
#define __EWINDOW_H__

/* popup isn't a real window */
#define WINDOWS_NUMBER 5

typedef enum {

	MENU=0,
	BAR=1,
	FILE_BOX=2,
	FILE_LINESNUMBER=3,
	FILE_CONTENT=4,
	POPUP=5

} WINDOW_TYPE;


typedef struct {

	size_t width;

	size_t height;

	unsigned int x;

	unsigned int y;

	unsigned int x_cursor;

	unsigned int y_cursor;

	WINDOW *window;

} eWindow;


eWindow *create_eWindow(size_t width, size_t height, unsigned int x, unsigned int y);
void delete_eWindow(eWindow **window);

#endif
