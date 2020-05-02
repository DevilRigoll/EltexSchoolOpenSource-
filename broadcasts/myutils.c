#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "myutils.h"


struct Msg genMsg() {
	srand((unsigned int) time(NULL));

	struct Msg msg;

	msg.len = rand() % (MAX_LENGTH-1);

	for (int i = 0; i < msg.len; ++i)
		//msg.text[i] = -127 + rand() % 127;
		msg.text[i] = 48 + rand() % 15;
	msg.text[msg.len] = '\0';

	msg.T = 1 + rand() % MAX_WAITING_TIME;
	
	return msg; 
}

void dieWithError(char *errorMessage) {
	perror(errorMessage);
	exit(1);
}
