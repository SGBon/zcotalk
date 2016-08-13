#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <string.h>
#include "setup.h"
#include "util.h"

void discover_interface(struct sockaddr *interface_result){
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	char* addr;

	getifaddrs(&ifap);
	int count = 0;
	/* list network interfaces */
	for(ifa = ifap; ifa; ifa = ifa->ifa_next){
		if(ifa->ifa_addr->sa_family==AF_INET){
			sa = (struct sockaddr_in *) ifa->ifa_addr;
			addr = inet_ntoa(sa->sin_addr);
			printf("%d. Interface: %s\tAddress: %s\n",count,ifa->ifa_name,addr);
			count++;
		}
	}

	/* get user's choice for interface */
	int choice = -1;
	printf("%d Network interfaces found; which interface do you want to use(%d-%d): ",count,0,count-1);
	while(1){
		scanf("%d",&choice);
		if(choice >= 0 && choice < count)
			break;
		else{
			printf("Value must be within range (%d-%d)\nTry again: ",0,count-1);
		}
	}
	printf("interface chosen %d\n",choice);

	/* get the struct for the interface to return it */
	//memset(&chosen, 0, sizeof(struct sockaddr)); // initialize
	count = 0;
	for(ifa = ifap; ifa; ifa = ifa->ifa_next){
		if(ifa->ifa_addr->sa_family==AF_INET){
			if(count == choice){
				memcpy(interface_result,ifa->ifa_broadaddr,sizeof(struct sockaddr));
				break;
			}
			count++;
		}
	}
	freeifaddrs(ifap);
}

int create_connection(struct sockaddr *interface, struct sockaddr_in *client){
	int s;
	if((s = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1){
		die("Failed to create client socket");
	}

	/* required so that the client can use broadcast IPs */
	int broadcastEnable = 1;
	setsockopt(s,SOL_SOCKET,SO_BROADCAST,&broadcastEnable,sizeof(broadcastEnable));

	/* initialize the client struct */
	memset((char *) client,0,sizeof(struct sockaddr_in));
	client->sin_family = AF_INET;
	client->sin_port = htons(UTPORT);

	/* set the client address to the broadcast address */
	struct sockaddr_in* bcast = (struct sockaddr_in*) interface;
	client->sin_addr = bcast->sin_addr;
	return s;
}

int create_listener(struct sockaddr_in *server){
	int s;
	socklen_t slen = sizeof(struct sockaddr_in);
	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1){
		die("Failed to create server socket");
	}

	/* initialize the struct, set fields */
	memset((char *) server,0,slen);
	server->sin_family = AF_INET;
	server->sin_port = htons(UTPORT);
	server->sin_addr.s_addr = htonl(INADDR_ANY);

	/* bind socket to port */
	if(bind(s,(struct sockaddr*) server,slen) == -1){
		die("Failed to bind server socket");
	}

	return s;
}

void destroy_connection(int clientd){
	close(clientd);
}
void destroy_listener(int serverd){
	close(serverd);
}
