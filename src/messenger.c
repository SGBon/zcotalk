#define _XOPEN_SOURCE 600

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

#define ARG_BARRIER_SIZE 2

static pthread_t listener_thread;
static pthread_mutex_t IO_lock;
static pthread_barrier_t arg_barrier; /* argument barrier */
static int listener_active = DISABLED;

/* struct for passing arguments to listener thread */
struct listener_args{
	int socket;
	WINDOW *ow;
	int height;
	int width;
};

static void *listener_cb(void *sid);

void listener_start(int socket, WINDOW *output_win, int height, int width){
	pthread_mutex_init(&IO_lock,NULL);
	if(pthread_barrier_init(&arg_barrier,NULL,ARG_BARRIER_SIZE)){
		die("Failed to initialize barrier in listener_start");
	}

	struct listener_args la;
	la.socket = socket;
	la.ow = output_win;
	la.height = height;
	la.width = width;

	pthread_create(&listener_thread,NULL,listener_cb, &la);
	/* argument may go out of scope before we it is obtained, so synchronize with barrier */
	pthread_barrier_wait(&arg_barrier);
	pthread_barrier_destroy(&arg_barrier);
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
	struct listener_args la = *(struct listener_args *) sid;
	int socket = la.socket;
	WINDOW *output_win = la.ow;
	const int maxheight = la.height - 1;
	const int maxwidth = la.width - 1;
	/* argument may go out of scope before we obtain it, so synchronize with barrier */
	pthread_barrier_wait(&arg_barrier);

	struct sockaddr_in incoming;
	socklen_t slen = sizeof(incoming);
	char buffer[BUFLEN];
	listener_active = ENABLED;

	int cheight = 1; /* height of cursor */
	while(listener_active){
		fflush(stdout);

		/* clear the buffer */
		memset(buffer,'\0',BUFLEN);

		/* blocking call, attempt to receive data */
		if(recvfrom(socket,buffer,BUFLEN,0,(struct sockaddr *) &incoming,&slen)==-1){
			die("Failed to receive message");
		}
		pthread_mutex_lock(&IO_lock);{
			wmove(output_win,cheight,1);
			wprintw(output_win,"%s:%d> %s\n",inet_ntoa(incoming.sin_addr),ntohs(incoming.sin_port),buffer);
			if(cheight == maxheight){
				wmove(output_win,1,1);
				wdeleteln(output_win);
			}else{
				cheight++;
			}
			box(output_win,0,0);
			wrefresh(output_win);
			pthread_mutex_unlock(&IO_lock);
		}
	}

	pthread_exit(NULL);
}

void messenger_start(int socket,struct sockaddr_in* client,WINDOW *input_win){
	char message[BUFLEN];
	socklen_t slen = sizeof(struct sockaddr_in);
	do{
		pthread_mutex_lock(&IO_lock);
		{
			/* bring cursor back to beginning, remove everything in input window */
			werase(input_win);
			box(input_win, 0 , 0);
			wmove(input_win,1,1);
			wprintw(input_win,">");
			wrefresh(input_win);
			pthread_mutex_unlock(&IO_lock);
		}
		wgetnstr(input_win,message,BUFLEN);
		if(sendto(socket,message,strlen(message),0,(struct sockaddr *) client,slen) == -1){
			die("Failed to send message");
		}
	}while(strcmp(message,"/quit\n"));
}
