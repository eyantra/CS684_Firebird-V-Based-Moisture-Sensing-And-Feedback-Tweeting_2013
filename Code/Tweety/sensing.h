unsigned char getSensorValue()
{
	unsigned char val = ADC_Conversion(14);
	lcd_print(2,1,val,3);
	return val;
}

void servo_dig_in()
{
	servo_2(105);
}

void servo_dig_out()
{
	servo_2(170);
}


unsigned int sense()
{
	int i,j,t;
	unsigned int median[3];
	servo_dig_in();
	_delay_ms(1500);
	median[0]=(unsigned int)getSensorValue();
	_delay_ms(1500);
	median[1]=(unsigned int)getSensorValue();
	_delay_ms(1500);
	median[2]=(unsigned int)getSensorValue();
	servo_dig_out();

	for(i=0;i<3;i++)
		for(j=i+1;j<3;j++)
		{
			if(median[i] > median[j])
			{
				t=median[i];
				median[i]=median[j];
				median[j]=t;
			}
		}
	return median[1];
}

unsigned char senseTrough(unsigned char dir)
{
	unsigned int moistVal = 0;
	if ( dir == 'r' )
	{
		right_degrees(85);
		_delay_ms(1000);
			moistVal = sense();
		_delay_ms(1000);
		right_degrees(5);
		_delay_ms(1000);
			moistVal += sense();
		_delay_ms(1000);
		right_degrees(5);
		_delay_ms(1000);
			moistVal += sense();
		_delay_ms(1000);
		left_upto_follow_line();

	}
	else
	{
		
		left_degrees(85);
		_delay_ms(1000);
			moistVal = sense();
		_delay_ms(1000);
		left_degrees(5);
		_delay_ms(1000);
			moistVal += sense();
		_delay_ms(1000);
		left_degrees(5);
		_delay_ms(1000);
			moistVal += sense();
		_delay_ms(1000);
		right_upto_follow_line();
	}
	return (unsigned char)(moistVal/3);
}
