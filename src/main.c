#include <stdio.h>
#include <pthread.h>
#include "setup.h"
#include "messenger.h"

int main(void){
	struct sockaddr interface;
	discover_interface(&interface);

	int client_socket;
	struct sockaddr_in client;
	client_socket = create_connection(&interface, &client);

	int server_socket;
	struct sockaddr_in server;
	server_socket = create_listener(&server);

	listener_start(server_socket);
	messenger_start(client_socket,&client);

	listener_end();

	destroy_connection(client_socket);
	destroy_listener(server_socket);
	pthread_exit(0);
}
