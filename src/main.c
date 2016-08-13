#include <stdio.h>
#include "setup.h"

int main(void){
	struct sockaddr interface;
	discover_interface(&interface);

	int client_socket;
	struct sockaddr_in client;
	client_socket = create_connection(&interface, &client);

	int server_socket;
	struct sockaddr_in server;
	server_socket = create_listener(&server);

	destroy_connection(client_socket);
	destroy_listener(server_socket);
	return 0;
}
