#include <stdlib.h>
#include <stdio.h>
#include "util.h"
void die(char *s){
	perror(s);
	exit(1);
}
