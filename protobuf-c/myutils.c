#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "myutils.h"

size_t packProtoBufMsg(struct Msg msg,uint8_t ** buf) {
	TcpMsg tcpmsg = TCP_MSG__INIT;

	size_t len = strlen(msg.text);
	tcpmsg.text = (char *)malloc(sizeof(char) * (len + 1));
	strcpy(tcpmsg.text, msg.text);
	tcpmsg.t = msg.T;
	tcpmsg.len = msg.len;	

	len = tcp_msg__get_packed_size(&tcpmsg);
	(*buf) = (uint8_t *)malloc(len * sizeof(uint8_t));
	tcp_msg__pack(&tcpmsg, *buf);

	return len;
}

struct Msg unpackProtoBufMsg(uint8_t * buf) {
	TcpMsg * tcpmsg;
	printf("3-1\n");
	size_t msg_len = strlen(buf);
	
	tcpmsg = tcp_msg__unpack(NULL, msg_len, buf);
	printf("3-2\n");
	struct Msg msg = {"", 0, 0};
	if (tcpmsg == NULL)
		return msg;
	strcpy(msg.text, tcpmsg->text);
	msg.len = tcpmsg->len;
	msg.T = tcpmsg->t;
	printf("3-3\n");
	tcp_msg__free_unpacked(tcpmsg, NULL);
	printf("3-4\n");
	return msg;
}

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
