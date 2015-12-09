#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include<string.h>

#include "serial.h"

#define DEV_NAME "/dev/ttymxc1"

int main(int argc, char *argv[]) {
	int fd;
	int len, ret;
	char buf[] = "hello";

	/*
	 * O_NOCTTY: program can not be control by keyboard etc.
	 * O_NDELAY: program don not care about DCD single line state
	 */
	fd = open(DEV_NAME, O_RDWR | O_NOCTTY/* | O_NDELAY*/);
	if (fd < 0) {
		perror("Open device error\n");
	}
	printf("Device opened : %s\n", DEV_NAME);

	/* 9600 8n1     150 255*/
	ret = set_port_attr(fd, B9600, 8, "1", 'N', 0, 0);
	if (ret < 0) {
		printf("Set attribute failed \n");
		exit(-1);
	}
	puts("Device attribute : (B9600 8n1)  (vtime 0)  (vmin 0)\n");

	len = write(fd, buf, sizeof(buf));
	if (len < 0) {
		printf("Write data error!\n");
		return -1;
	}
	printf("Output data : %s \n", buf);

	memset(buf, 0, sizeof(buf));
	len = read(fd, buf, sizeof(buf));
	if (len < 0) {
		printf("Read data error!\n");
		return -1;
	}
	printf("Read length : %d \n", len);
	printf("Read data : %s \n", buf);

	close(fd);
	printf("Device closed  : %s\n", DEV_NAME);

	return (0);
}

