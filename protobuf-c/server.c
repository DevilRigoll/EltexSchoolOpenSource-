#include <stdio.h>      
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for sleep() */
#include <pthread.h>


#include "mytypes.h"
#include "queue.h"
#include "myutils.h"


struct SharedData shared;

void * tcpHandlerType1(void * args) {	
	int servSock;                    
	int clntSock;                    
	struct sockaddr_in servAddr; 
	struct sockaddr_in clntAddr;    
	unsigned int clntLen;
	
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        	dieWithError("tcp_socket() failed"); 

	memset(&servAddr, 0, sizeof(servAddr));   
    	servAddr.sin_family = AF_INET;    
    	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	servAddr.sin_port = htons(tcpPortT1); 

	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        	dieWithError("bind() failed");

	if (listen(servSock, MAXPENDING) < 0)
        	dieWithError("listen() failed");

	while(1) {
		clntLen = sizeof(clntAddr);
		if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, 
                       &clntLen)) < 0)
    			dieWithError("accept() failed");
		
		pthread_mutex_lock(&shared.mutex);

		if (shared.queue.size < QUEUE_MAX_SIZE) {
			size_t recvMsgSize = 0;
	
			char buffer[MAX_LENGTH];
			uint8_t * buf = NULL;
			size_t len = 0;                  

    			if ((recvMsgSize = recv(clntSock, buffer, MAX_LENGTH, 0)) < 0)
        			dieWithError("recv() failed");
			
			len += recvMsgSize;
			buf = (uint8_t *)malloc(len * sizeof (uint8_t));
			for (int i = 0; i < len; ++i)
				buf[i] = buffer[i];

			printf("1-1\n");

    			while (recvMsgSize > 0) {

				if ((recvMsgSize = recv(clntSock, buffer, MAX_LENGTH, 0)) < 0)
				    dieWithError("recv() failed");

				buf = realloc(buf, (len + recvMsgSize) * sizeof (uint8_t));
				for (int i = 0; i < recvMsgSize; ++i)
					buf[i + len] = buffer[i];
				
				len += recvMsgSize;
				
			}

			printf("1-2\n");
			struct Msg msg;
			msg = unpackProtoBufMsg(buf);

			free(buf);
			
			qPushBack(&shared.queue, msg);

		}

		pthread_mutex_unlock(&shared.mutex);

		close(clntSock);
	}

	close(servSock);       
}


void * tcpHandlerType2(void * args) {	
	int servSock;                    
	int clntSock;                    
	struct sockaddr_in servAddr; 
	struct sockaddr_in clntAddr;    
	unsigned int clntLen;
	

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        	dieWithError("tcp_socket() failed"); 
	
	memset(&servAddr, 0, sizeof(servAddr));   
    	servAddr.sin_family = AF_INET;    
    	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	servAddr.sin_port = htons(tcpPortT2);

	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        	dieWithError("bind() failed");

	if (listen(servSock, MAXPENDING) < 0)
        	dieWithError("listen() failed");

	while(1) {
		clntLen = sizeof(clntAddr);
		if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr, 
                       &clntLen)) < 0)
    			dieWithError("accept() failed");

		pthread_mutex_lock(&shared.mutex);
		printf("2-1\n");
		if (shared.queue.size > 0) {
			struct Msg msg;
			qPopFront(&shared.queue, &msg);

			uint8_t * buf = NULL;
			size_t size = packProtoBufMsg(msg, &buf);
			long bytes = 0;
			if ((bytes = send(clntSock, buf, size, 0)) != size)
        			dieWithError("send() sent a different number of bytes than expected");

			free(buf);	
		}
		printf("2-2\n");
		pthread_mutex_unlock(&shared.mutex);
    		close(clntSock);
	}
	
	close(servSock);       
}


void * broadcastType1(void * args) {
	int sock;                         
	struct sockaddr_in broadcastAddr;               
	int broadcastPermission;          
	unsigned int sendStringLen;

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        	dieWithError("socket() failed");

	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        	dieWithError("setsockopt() failed");

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   
    	broadcastAddr.sin_family = AF_INET;    
    	broadcastAddr.sin_addr.s_addr = inet_addr(serverIP); 
    	broadcastAddr.sin_port = htons(broadcastPortT1);

	sendStringLen = strlen(broadcast_cl1_msg);

	unsigned int size = 0;
	while(1) {
		pthread_mutex_lock(&shared.mutex);
		size = shared.queue.size;
		pthread_mutex_unlock(&shared.mutex);

		printf("Queue size b1 = %d\n", size);

		if (size < QUEUE_MAX_SIZE)
		if (sendto(sock, broadcast_cl1_msg, sendStringLen, 0, (struct sockaddr *) 
               	    &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
             		dieWithError("sendto() sent a different number of bytes than expected");

        	sleep(SERV_BROADCAST_WAIT_T1);
	}

	close(sock);
}

void * broadcastType2(void * args) {
	int sock;                         
	struct sockaddr_in broadcastAddr;               
	int broadcastPermission;          
	unsigned int sendStringLen;

	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        	dieWithError("socket() failed");

	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        	dieWithError("setsockopt() failed");

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   
    	broadcastAddr.sin_family = AF_INET;    
    	broadcastAddr.sin_addr.s_addr = inet_addr(serverIP); 
    	broadcastAddr.sin_port = htons(broadcastPortT2);


	sendStringLen = strlen(broadcast_cl2_msg);
	unsigned int size = 0;
	while(1) {
		pthread_mutex_lock(&shared.mutex);
		size = shared.queue.size;
		pthread_mutex_unlock(&shared.mutex);

		printf("Queue size b2 = %d\n", size);

		if (size > 0)
		if (sendto(sock, broadcast_cl2_msg, sendStringLen, 0, (struct sockaddr *) 
               	    &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
             		dieWithError("sendto() sent a different number of bytes than expected");

        	sleep(SERV_BROADCAST_WAIT_T2);
	}
	
	close(sock);
}



int main(int argc, char *argv[]){
	
	shared.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	shared.queue = initQ();

	pthread_t tpc_client1_thread;
	pthread_create(&tpc_client1_thread, NULL, tcpHandlerType1, NULL);

	pthread_t tpc_client2_thread;
	pthread_create(&tpc_client2_thread, NULL, tcpHandlerType2, NULL);

	pthread_t broadcast2_thread;
	pthread_create(&broadcast2_thread, NULL, broadcastType2, NULL);
	
	broadcastType1(NULL);
	        
      	pthread_cancel(broadcast2_thread);
	pthread_cancel(tpc_client1_thread);
	pthread_cancel(tpc_client2_thread);

	qClear(&shared.queue);
	return 0;
}
