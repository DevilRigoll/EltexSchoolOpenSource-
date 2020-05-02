#include <stdio.h>      
#include <sys/socket.h> 
#include <arpa/inet.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

#include "mytypes.h"
#include "myutils.h"

int main(int argc, char * argv[]) {	
	//int sock;                        
	struct sockaddr_in broadcastAddr; 
	char recvString[MAX_LENGTH+1];
	int recvStringLen;                

	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   
    	broadcastAddr.sin_family = AF_INET;    
    	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    	broadcastAddr.sin_port = htons(broadcastPortT2);      

	while(1) {
		int sock;
		
		if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		dieWithError("socket() failed");

		const int broadcastPermission = 1;
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &broadcastPermission, 
		  sizeof(broadcastPermission)) < 0)
			dieWithError("setsockopt1() failed");
	
		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
		  sizeof(broadcastPermission)) < 0)
			dieWithError("setsockopt2() failed");

		if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
		dieWithError("bind() failed");
		
		if ((recvStringLen = recvfrom(sock, recvString, MAX_LENGTH, 0, NULL, 0)) < 0)
			dieWithError("recvfrom() failed");

		close(sock);
		
		recvString[recvStringLen] = '\0';
		printf("Received: %s\n", recvString);

		if (!strncmp(recvString, broadcast_cl2_msg, recvStringLen)) {			
			int tcpsock;
			struct sockaddr_in tcpServAddr;

			if ((tcpsock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        			dieWithError("tcp_socket() failed");
			if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *) &broadcastPermission, 
          		   sizeof(broadcastPermission)) < 0)
        			dieWithError("setsockopt1() failed");			
			
			memset(&tcpServAddr, 0, sizeof(tcpServAddr));   
    			tcpServAddr.sin_family = AF_INET;    
    			tcpServAddr.sin_addr.s_addr = inet_addr(serverIP);
    			tcpServAddr.sin_port = htons(tcpPortT2);

			if (connect(tcpsock, (struct sockaddr *) &tcpServAddr, sizeof(tcpServAddr)) < 0)
        			dieWithError("connect() failed");
			
			long bytes = 0;
			struct Msg msg;
			if ((bytes = recv(tcpsock, &msg, sizeof(struct Msg), 0)) != sizeof(struct Msg)) {
				printf("send() error %ld != %ld", bytes, sizeof(struct Msg));
        			dieWithError("send() sent a different number of bytes than expected");
			}

			printf("MSG = %s\n", msg.text);		

			close(tcpsock);
			
			sleep(msg.T);
		}   

	}	

	return 0;
} 
