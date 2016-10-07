#ifndef ZCOTALK_WM_H
#define ZCOTALK_WM_H
#include <ncurses.h>

/* Window Management, for creating and destroying ncurses windows
* based off of code from
* http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
*/

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
#endif
