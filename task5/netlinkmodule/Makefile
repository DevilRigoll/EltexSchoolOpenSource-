CC=gcc
CFLAG= -o
obj-m += netlink.o
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm usernetlink

usernetlink: usernetlink.c
	${CC} -Wall ${CFLAG} usernetlink usernetlink.c

test:
	sudo insmod netlink.ko
	sudo dmesg | tail -1
	./usernetlink
	sudo dmesg | tail -1
	sudo rmmod netlink.ko
	sudo dmesg | tail -1
