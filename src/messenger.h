#ifndef UOITALK_MESSENGER_H
#define UOITALK_MESSENGER_H
#include<netinet/in.h>

/* starts a thread which begins listening to messages
and prints them to the screen */
void listener_start(int socket);

/* joins the listener thread */
void listener_end();

/* handles user input and sends over the network */
void messenger_start(int socket,struct sockaddr_in* client);

#endif
