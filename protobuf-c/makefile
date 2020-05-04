CC=gcc
CFLAG= -c
TERMINAL=gnome-terminal -e

build: server client1 client2

test: 
	${TERMINAL} "${PWD}/client1"
	${TERMINAL} "${PWD}/client1"
	${TERMINAL} "${PWD}/client1"
	${TERMINAL} "${PWD}/client2"
	${TERMINAL} "${PWD}/client2"
	${TERMINAL} "${PWD}/client2"
	${TERMINAL} "${PWD}/server"

server: server.c queue.o myutils.o msg.pb-c.o mytypes.h
	${CC} -Wall ${CFLAG} -o server.o server.c -lpthread
	${CC} -o server server.o queue.o myutils.o msg.pb-c.o -lpthread -lprotobuf-c

client1: client1.c myutils.o msg.pb-c.o mytypes.h
	${CC} -Wall ${CFLAG} -o client1.o client1.c
	${CC} -o client1 client1.o myutils.o msg.pb-c.o -lprotobuf-c

client2: client2.c myutils.o msg.pb-c.o mytypes.h
	${CC} -Wall ${CFLAG} -o client2.o client2.c
	${CC} -o client2 client2.o myutils.o msg.pb-c.o -lprotobuf-c

queue.o: queue.c queue.h mytypes.h
	${CC} ${CFLAG} queue.c

myutils.o: msg.pb-c.o myutils.c myutils.h mytypes.h
	${CC} ${CFLAG} myutils.c

msg.pb-c.o: msg.pb-c-files
	${CC} ${CFLAG} msg.pb-c.c -lprotobuf-c

msg.pb-c-files: msg.proto
	protoc --c_out=. msg.proto

clean:
	rm -f server server.o
	rm -f client1 client1.o
	rm -f client2 client2.o
	rm -f myutils.o queue.o
	rm -f msg.pb-c.o msg.pb-c.c msg.pb-c.h
