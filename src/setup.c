#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <string.h>
#include "setup.h"

struct sockaddr connection_setup(){
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
	printf("%d Network interfaces found; which interface do you want to use(%d-%d): ",count,0,count);
	while(1){
		scanf("%d",&choice);
		if(choice >= 0 && choice <= count)
			break;
		else{
			printf("Value must be within range (%d-%d)\nTry again: ",0,count);
		}
	}
	printf("interface chosen %d\n",choice);

	/* get the struct for the interface to return it */
	struct sockaddr chosen;
	memset(&chosen, 0, sizeof(chosen)); // initialize
	count = 0;
	for(ifa = ifap; ifa; ifa = ifa->ifa_next){
		if(ifa->ifa_addr->sa_family==AF_INET){
			if(count == choice){
				memcpy(&chosen,ifa->ifa_broadaddr,sizeof(chosen));
				break;
			}
			count++;
		}
	}
	freeifaddrs(ifap);
	return chosen;
}
