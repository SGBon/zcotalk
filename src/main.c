#include <stdio.h>
#include "util.h"
#include "setup.h"

int main(void){
	struct sockaddr interface;
	struct sockaddr_in client;
	int s;
	discover_interface(&interface);

	s = create_connection(&interface, &client);
	printf("%d\n",s);
	return 0;
}
