#ifndef ZCOTALK_SETUP_H
#define ZCOTALK_SETUP_H
#include <netinet/in.h>

/* query user about interface to use for broadcasting */
void discover_interface(struct sockaddr *interface_result);

/* create the connection to broadcast to
in other words, the client of client-server */
int create_connection(struct sockaddr *interface, struct sockaddr_in *client);

/* create the the listener which will receive messages */
int create_listener(struct sockaddr_in *server);

/* closes file descriptors */
void destroy_connection(int clientd);
void destroy_listener(int serverd);

#endif
