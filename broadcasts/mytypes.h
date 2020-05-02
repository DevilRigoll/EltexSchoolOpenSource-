#ifndef _MY_TYPES_
#define _MY_TYPES_

#define MAX_LENGTH 1024
#define MAX_WAITING_TIME 20
#define SERV_BROADCAST_WAIT_T1 7
#define SERV_BROADCAST_WAIT_T2 4
#define MAXPENDING 255
#define QUEUE_MAX_SIZE 5

#define broadcastPortT1 8000
#define broadcastPortT2 8800
#define tcpPortT1 8880
#define tcpPortT2 8888
#define serverIP "127.0.0.1"
#define broadcast_cl1_msg "Waiting a msg"
#define broadcast_cl2_msg "Have a msg"

struct Msg {
	char text[MAX_LENGTH];
	unsigned int len;
	unsigned int T;
};
typedef struct Msg Msg;

struct Node {
	struct Node * next;
	Msg elem;
};

struct Queue {
	struct Node * head;
	struct Node * tail;
	unsigned int size;
};
typedef struct Queue Queue;

struct SharedData {
	pthread_mutex_t mutex;
	Queue queue; 

};

#endif
