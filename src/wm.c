#include "wm.h"
/* based off of code from
* http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
*/

WINDOW *create_newwin(int height, int width, int starty, int startx){
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0 , 0);
  wrefresh(local_win); /* Show the box */

  return local_win;
}

void destroy_win(WINDOW *local_win){
  /* box(local_win, ' ', ' '); : This won't produce the desired
  * result of erasing the window. It will leave it's four corners
  * and so an ugly remnant of window.
  */
  wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

  wrefresh(local_win);
  delwin(local_win);
}
