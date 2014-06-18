#include<avr/io.h> 
#include<avr/interrupt.h>
#include<util/delay.h>
#include "lcd.h"
#include "tcommon.h"
#include "zigbee.h"
#include "navigation.h"
#include "sensing.h"
#include "math.h"

//#define F_CPU 14745600

unsigned char oldCheckPoint=0;
unsigned char oldLinePoint=0;
unsigned char checkPoint = 0;
unsigned char linePoint = 0;
unsigned char distance = 0;
unsigned char oldDistance = 0;
unsigned char direction = 0;
unsigned char moistureValue = 0;

int lack=0;


void init_devices()
{
 	cli(); //Clears the global interrupts
	lcd_port_config();
	lcd_init();
	buzzer_pin_config();
	adc_pin_config();
	motion_pin_config();
	left_encoder_pin_config();
	left_position_encoder_interrupt_init();
	right_encoder_pin_config();
 	right_position_encoder_interrupt_init();
	adc_init();
	adc_init_10bit();
	servo2_pin_config();
	timer5_init();
	timer1_init();
	init_zigbee();
	//init_navigation();
	sei();   //Enables the global interrupts
}

int main()
{
	
	unsigned int flagOffset = 0;		// 1 => next location for same checkpoint
	//unsigned char olddistance=0;		// distance travelled so far after a checkpoint
	init_devices();
	servo_dig_out();
	buzzer_on();
	_delay_ms(500);
	buzzer_off();
	while(1)
	{
		flagOffset = 0;
		//print_sensor(1,5,2);
		oldLinePoint = linePoint;		// save last linePoint
		linePoint = readZigbee();		// read new linePoint
		lcd_reset();
		lcd_cursor(1,1);
		lcd_string("LinePoint:");
		lcd_cursor(2,1);
		lcd_wr_char(linePoint + '0');
		_delay_ms(1000);

		if(linePoint!=oldLinePoint)		//	return from this line and sit facing new line
		{
			goToLine(oldLinePoint,linePoint,checkPoint);
			checkPoint=0;
			if(linePoint==0)
			{
				sendSignal(linePoint);
				continue;
			}
		}
		if(linePoint==0 && oldLinePoint==0)
		{
			sendSignal(linePoint);
			continue;
		}

		sendSignal(linePoint);			// send back this new checkpoint to zigbee server
		oldCheckPoint = checkPoint;		// save last checkpoint
		checkPoint = readZigbee();		// read new checkpoint
		lcd_reset();
		lcd_cursor(1,1);
		lcd_string("Checkpoint:");
		lcd_cursor(2,1);
		lcd_wr_char(checkPoint + '0');
		_delay_ms(1000);

		if(oldCheckPoint == checkPoint)	// no need to go to next checkpoint
		{
			flagOffset = 1;
		}
		else
		{
			goToCheckPoint(oldCheckPoint,checkPoint);
			oldDistance = 0;
		}
		sendSignal(checkPoint);			// send back this new checkpoint to zigbee server
		oldDistance += distance;
		distance = readZigbee();
		
		if (flagOffset)
			distance = distance - oldDistance;		// distance to be travelled is relative to current position
		lcd_reset();
		lcd_cursor(1,1);
		lcd_string("Distance:    ");
		lcd_print(2,1,distance,3);
		//_delay_ms(1000);
		forward_follow_mm((((unsigned int)distance)*10)+(botLength*(1-flagOffset)));//((unsigned int)distance )*10);
		sendSignal(distance);
		direction = readZigbee();					// l or r
		sendSignal(direction);
		lcd_reset();
		lcd_cursor(1,1);
		lcd_string("Direction:");
		lcd_cursor(2,1);
		lcd_wr_char(direction);
		moistureValue=senseTrough(direction);
		lcd_reset();
		lcd_cursor(1,1);
		lcd_string("Moisture Value:");
		lcd_cursor(2,1);
		lcd_print(2,1,moistureValue,3);
		sendSignal(moistureValue);

	}
}
