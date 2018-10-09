#include <string.h>
#include <fcntl.h>
#include "termios.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define  DEVICE_NAME "/dev/ttyUSB0"
int uart_fd = 0;
static int uart_open() {
	int fd = open(DEVICE_NAME, O_RDWR);
	if (fd == -1) {
		printf("open device %s error\n", DEVICE_NAME);
		return fd;
	}
	printf("Serial open %s success fd = %d\n", DEVICE_NAME, fd);

	return fd;
}

static speed_t getBaudrate(int baudrate) {
	switch (baudrate) {
	case 0:
		return B0;
	case 50:
		return B50;
	case 75:
		return B75;
	case 110:
		return B110;
	case 134:
		return B134;
	case 150:
		return B150;
	case 200:
		return B200;
	case 300:
		return B300;
	case 600:
		return B600;
	case 1200:
		return B1200;
	case 1800:
		return B1800;
	case 2400:
		return B2400;
	case 4800:
		return B4800;
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default:
		return -1;
	}
}
static int uart_setup(int fd, int baudrate, int nbits, int parary,int stopbits) {
        struct termios new_termios, old_termios;
        int ret;

	tcgetattr(fd, &old_termios);
	new_termios = old_termios;
	new_termios.c_lflag &= ~(ICANON | ECHO | ISIG);
	new_termios.c_cflag |= (CREAD | CLOCAL);
	new_termios.c_cflag &= ~(CSTOPB | PARENB | CRTSCTS);
	new_termios.c_cflag &= ~(CBAUD | CSIZE) ;
	new_termios.c_cflag |= (B115200 | CS8);
	ret = tcsetattr(fd, TCSANOW, &new_termios);
        if(ret < 0) {
            printf("Configure device tcsetattr() failed 2\n");
	    close(fd);
	    return fd;
	}

	return fd;
}

static int uart_write(int fd, char* send_data, int send_size) {
	int index = 0;
	int isCopy;

	int iLen = write(fd, send_data, send_size);
	printf("uart_write iLen=%d\n", iLen);
	for (index = 0; index < iLen; index++) {
		printf("data[%d]=0x%x,", index, send_data[index]);
	}
        printf("\n");
	return iLen;
}
int uart_recv(int fd, char *data, int datalen) {
	int ret = 0;
	int i;
	fd_set fs_read;
	struct timeval tv_timeout;
	int DataLength;

	FD_ZERO(&fs_read);
	FD_SET(fd, &fs_read);
	tv_timeout.tv_sec = 2;
	tv_timeout.tv_usec = 0;

	//printf("[%s] fd = %d \n", __func__, fd);
	ret = select(fd + 1, &fs_read, NULL, NULL, NULL);

	if (ret < 0) {
		printf("select error ret = %d\n", ret);
		return ret;
	} else if (ret == 0) {
		printf("uart_recv timeout\n");
		return ret;
	} else {
		//printf("uart_recv ret = %d\n", ret);
	}

	if (FD_ISSET(fd, &fs_read)) {
		DataLength = read(fd, data, datalen);
		//printf("read ... DataLength = %d\n", DataLength);
		return DataLength;
	} else {
		printf("select wait ...\n");
		return -1;
	}

	return 0;
}
#define LENGTH 256
static int uart_read(int fd, char* msg_data, int size, int rcv_wait) {
	int index = 0, iLen = 0;
	char c_msg_data[LENGTH];
	memset(c_msg_data, 0, LENGTH);

	size = uart_recv(fd, c_msg_data, LENGTH);
        printf("%s",c_msg_data);
	if (size > 0) {
		//printf("uart_read size = %d\n", size);

		for (index = 0; index < size; index++) {
			//printf("c_msg_data[%d]=0x%x,", index, c_msg_data[index]);
		}
                //printf("\n");
	} else {
                sleep(3);
		printf("uart_read timeOut\n");
	}
	return size;
}

static void uart_close(int fd) {
	close(fd);
	printf("Serial close success\n");
}

static void *recv_thread_func(void* arg)
{
    char msg_data[1024];
    int recv_size = 0;
    //printf("recv_thread (arg=%d tid=%d) entered", (unsigned)arg, gettid());
    if (uart_fd > 0) {
	while (1) {
		recv_size = uart_read(uart_fd, msg_data, 1024, 0);
                //printf("recv_size = %d\n",recv_size);
	}
    }
    return 0;
}
#if 0
int main(int argc, char **argv) {
#else
int uartMain() {
	char* sendCmd = "ATI\n";
	int sendSize = 0;
        pthread_t recv_thread, send_thread;

	uart_fd = uart_open();
	uart_setup(uart_fd, 115200, 8, "N", 1);

	sendSize = sizeof(sendCmd);
	printf("sendSize = %d", sendSize);

        pthread_create(&recv_thread, NULL, recv_thread_func, (void *)1);
        sleep(3);
        uart_write(uart_fd,sendCmd,sendSize);
        pthread_join(recv_thread, NULL);
	return 0;
}
#endif
