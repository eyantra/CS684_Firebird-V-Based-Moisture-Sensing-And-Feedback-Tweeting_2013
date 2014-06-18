#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  /* For atoi function */

/**
 *  Open port for read/write
 */
int open_port(void)
{
	int fd;
	/* Open Port for Zigbee communication */
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		perror("open_port: Unable to open /dev/ttyUSB0 - ");
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
	int fd = open("/dev/ttyUSB0", O_RDONLY | O_NOCTTY);
	if (fd == -1)
	{
		/* Could not open the port. */
		perror("open_port: Unable to open /dev/ttyUSB0 - ");
	}
	unsigned char buffer;
	int n = read(fd, &buffer, 1);
	if (n < 0)
		fputs("read failed!\n", stderr);
	printf("%d\n", buffer);
	return (int)(buffer);
}

/**
 *  The sequence of write signals for the entire sequence of sensing
 *  activity commands over Zigbee port.
 */
int write_port(int fd)
{
	char buf[10000],intStr[100],fname[20];
	unsigned char hchk[1],vchk[1],ofst[1];
	FILE *outfile;
	FILE *file;
	int n,a;
	char* pch;
	int i=0,sensed;

	/* Open the file containing the sorted list of sensing locations */

	file =fopen("locations.csv","r");
	if (!file)
		printf("error in file open");
	int sensedCtr = 1; 
	while (fgets(buf,10000, file)!=NULL)
        {
		/*
		   Read a line from the location file.
		   Each line is of the form <X,Y,dist,dir>
		   X	=> The lane where sensing must be performed.
		   Y	=> The checkpoint within that lane.
		   dist	=> Distance in cm to be travelled after the checkpoint
		   dir	=> Specifying whether the sensing has to be performed in trough
			   on l(left) or r(right) of lane.
		*/
		puts(buf);
		pch = strtok (buf,",");	
		a = atoi(pch);
		hchk[0] = (unsigned char)a;
		printf("\n%d",hchk[0]);

		/* Write single byte for specifying lane number on Zigbee port */
		n = write(fd,hchk, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);

		/* Wait for acknowledgement */
		read_port();
		/* Lane number 0 indicates end of sensing schedule */
		if(a == 0)
			break;
		
		pch = strtok (NULL,",");
		a = atoi(pch);
		vchk[0] = (unsigned char)a;
		printf("\n%d",vchk[0]);

		/* Write single byte for specifying checkpoint number */
		n = write(fd,vchk, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);

		/* Wait for acknowledgement */
		read_port();
		
		pch = strtok (NULL,",");
		a = atoi(pch);
		ofst[0] = (unsigned char)a;
		printf("\n%d",ofst[0]);

		/* Write single byte for specifying distance in cm */
		n = write(fd,ofst, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);

		/* Wait for acknowledgement */
		read_port();
		
		pch = strtok (NULL,",");
		printf("\n%s",pch);

		/* Write single byte for specifying direction */
		n = write(fd,pch, 1);
		if (n < 0)
			fputs("write() of 1 bytes failed!\n", stderr);

		/* Wait for acknowledgement */
		read_port();
		printf("Sensor Reading:");

		/* Read sensor value sent by the bot over Zigbee */
		sensed = read_port();
		sprintf(intStr,"%d",sensed);

		/* Write sensed value to a temporary file */
		sprintf(fname,"sensed%d.csv",sensedCtr++);
		outfile = fopen(fname,"w");
		fputs(intStr, outfile);
		fclose(outfile);
	}
	outfile = fopen("senseDone.txt","w");
	fclose(outfile);
	fclose(file);
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
	tty.c_cflag     &=  ~( PARENB | PARODD );   // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;               // no flow control
	tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;         // turn on READ & ignore ctrl lines

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
