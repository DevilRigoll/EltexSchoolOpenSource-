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

server: server.c queue.o myutils.o mytypes.h
	${CC} -Wall ${CFLAG} -o server.o server.c -lpthread
	${CC} -o server server.o queue.o myutils.o -lpthread

client1: client1.c myutils.o mytypes.h
	${CC} -Wall ${CFLAG} -o client1.o client1.c
	${CC} -o client1 client1.o myutils.o

client2: client2.c myutils.o mytypes.h
	${CC} -Wall ${CFLAG} -o client2.o client2.c
	${CC} -o client2 client2.o myutils.o

queue.o: queue.c queue.h mytypes.h
	${CC} ${CFLAG} queue.c

myutils.o: myutils.c myutils.h mytypes.h
	${CC} ${CFLAG} myutils.c

clean:
	rm server server.o
	rm client1 client1.o
	rm client2 client2.o
	rm myutils.o queue.o
