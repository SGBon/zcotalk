#include <stdio.h>
#include <pthread.h>
#include "setup.h"
#include "messenger.h"
#include "wm.h"

int main(void){
	struct sockaddr interface;
	discover_interface(&interface);

	int client_socket;
	struct sockaddr_in client;
	client_socket = create_connection(&interface, &client);

	int server_socket;
	struct sockaddr_in server;
	server_socket = create_listener(&server);

	initscr();

	/* input window size parameters */
	const int IW_H = 4;
	const int IW_W = COLS - 1;
	const int IW_SY = LINES - IW_H;
	const int IW_SX = 0;

	/* create our input window */
	WINDOW *iw = create_newwin(IW_H,IW_W, IW_SY,IW_SX);

	/* output window size parameters */
	const int OW_H = LINES - IW_H;
	const int OW_W = COLS - 1;
	const int OW_SY = 0;
	const int OW_SX = 0;

	/* create our output window */
	WINDOW *ow = create_newwin(OW_H,OW_W,OW_SY,OW_SX);

	listener_start(server_socket,ow,OW_H,OW_W);
	messenger_start(client_socket,&client,iw);

	listener_end();

	destroy_win(iw);
	destroy_win(ow);

	destroy_connection(client_socket);
	destroy_listener(server_socket);

	endwin();
	pthread_exit(0);
}
