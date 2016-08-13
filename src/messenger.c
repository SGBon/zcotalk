#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "messenger.h"
#include "util.h"

#define ENABLED 1
#define DISABLED 0

#define BUFLEN 512

static pthread_t listener_thread;
static pthread_mutex_t IO_lock;
static int listener_active = DISABLED;

void *listener_cb(void *sid);

void listener_start(int socket){
	pthread_mutex_init(&IO_lock,NULL);
	pthread_create(&listener_thread,NULL,listener_cb,(void *)socket);
}

void listener_end(){
	{
		pthread_mutex_lock(&IO_lock);
		listener_active = DISABLED;
		pthread_mutex_unlock(&IO_lock);
	}
	pthread_cancel(listener_thread);
	pthread_mutex_destroy(&IO_lock);
}

/* callback function used by the listener thread */
void *listener_cb(void *sid){
	int socket = (int) sid;
	struct sockaddr_in incoming;
	socklen_t slen = sizeof(incoming);
	char buffer[BUFLEN];
	listener_active = ENABLED;

	while(listener_active){
		fflush(stdout);

		/* clear the buffer */
		memset(buffer,'\0',BUFLEN);

		/* blocking call, attempt to receive data */
		if(recvfrom(socket,buffer,BUFLEN,0,(struct sockaddr *) &incoming,&slen)==-1){
			die("Failed to receive message");
		}
		pthread_mutex_lock(&IO_lock);{
			printf("%s:%d> %s\n",inet_ntoa(incoming.sin_addr),ntohs(incoming.sin_port),buffer);
			pthread_mutex_unlock(&IO_lock);
		}
	}

	pthread_exit(NULL);
}

void messenger_start(int socket,struct sockaddr_in* client){
	char message[BUFLEN];
	socklen_t slen = sizeof(struct sockaddr_in);
	do{
		pthread_mutex_lock(&IO_lock);
		{
			printf(">");
			pthread_mutex_unlock(&IO_lock);
		}
		fgets(message,BUFLEN,stdin);
		if(sendto(socket,message,strlen(message),0,(struct sockaddr *) client,slen) == -1){
			die("Failed to send message");
		}
	}while(strcmp(message,"/quit\n"));
}
