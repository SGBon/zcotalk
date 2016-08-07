#ifndef UOITALK_SETUP_H
#define UOITALK_SETUP_H
#include <netinet/in.h>

/* query user about interface to use for broadcasting */
void discover_interface(struct sockaddr *interface_result);

/* create the connection to broadcast to
in other words, the client of client-server */
int create_connection(struct sockaddr *interface, struct sockaddr_in *client);

#endif
