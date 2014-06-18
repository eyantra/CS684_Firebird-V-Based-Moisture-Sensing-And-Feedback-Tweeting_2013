
volatile unsigned char data; //to store received data from UDR1
int bflag = 0;
volatile int rcvFlag = 0;

void uart0_init(void)
{
 UCSR0B = 0x00; //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;
 UBRR0L = 0x5F; //set baud rate lo
 UBRR0H = 0x00; //set baud rate hi
 UCSR0B = 0x98;
}


SIGNAL(SIG_USART0_RECV) 		// ISR for receive complete interrupt
{
	rcvFlag = 1;
	data = UDR0; 				//making copy of data from UDR0 in 'data' variable 
}

void sendSignal(char msg)
{
	UDR0 = msg;
}


//Function To Initialize all The Devices
void init_zigbee()
{
 cli(); //Clears the global interrupts
 uart0_init(); //Initailize UART1 for serial communiaction
 sei();   //Enables the global interrupts
}

unsigned char readZigbee()
{
	while(!rcvFlag);
	rcvFlag=0;
	return data;
}

