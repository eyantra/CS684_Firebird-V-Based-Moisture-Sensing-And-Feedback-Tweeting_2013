#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  /* For using the function atoi */

/**
 *  Open port for read/write
 */
int open_port(void)
{
	int fd;
	/* Open Port for Zigbee communication */
	fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror("open_port: Unable to open /dev/ttyUSB1 - ");
	}
	else
		fcntl(fd, F_SETFL, 0);
	return (fd);
}

/**
 *  Read single byte over Zigbee and return it as an int
 */
int read_port(void)
{
	/* Open Port for reading single byte */
	int fd = open("/dev/ttyUSB1", O_RDONLY | O_NOCTTY);
	if (fd == -1)
	{
		/* Could not open the port. */
		perror("open_port: Unable to open /dev/ttyUSB1 - ");
	}
	char buffer;
	int n = read(fd, &buffer, 1);
	if (n < 0)
		fputs("read failed!\n", stderr);
	printf("%d\n", buffer);
	return (int)(buffer);
}

/**
 *  Write single byte signals for starting and stopping water pump
 */
int write_port(int fd)
{
		unsigned char ofst[1];
		ofst[0] = 0x38;
		int n = write(fd, ofst, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);
		sleep(15);
		ofst[0] = 0x35;
		n = write(fd, ofst, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);
		return 0;
}
int main()
{
	int fd;
	// char buffer[1];

	struct termios tty;

	struct termios tty_old;
	memset (&tty, 0, sizeof tty);
	fd=open_port();
	if ( tcgetattr ( fd, &tty ) != 0 )
	{
		printf("Error ");
	}
	/* Save old tty parameters */
	tty_old = tty;
	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~( PARENB | PARODD );        // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( fd, TCIFLUSH );
	if ( tcsetattr ( fd, TCSANOW, &tty ) != 0)
	{
		printf ("Error " );
	}
	write_port(fd);

	close(fd);
}

