#ifndef ZCOTALK_MESSENGER_H
#define ZCOTALK_MESSENGER_H
#include <netinet/in.h>
#include <ncurses.h>

/* starts a thread which begins listening to messages
and prints them to the ncuses window pointed to by
output_win */
void listener_start(int socket, WINDOW *output_win, int height, int width);

/* joins the listener thread */
void listener_end();

/* handles user input from ncurses window input_win and sends over the network */
void messenger_start(int socket,struct sockaddr_in* client, WINDOW *input_win);

#endif
