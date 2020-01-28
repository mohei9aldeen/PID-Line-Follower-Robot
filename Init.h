/*
*  init all of the usual needed functions
*/


#define LEFT_MOTOR                                     PD3 // 3  
#define RIGHT_MOTOR                                    PB3 // 11

//  motor direction Pin's ....
#define LEFT_FORWARD                                   PD4 // 4
#define LEFT_BACKWORD                                  PD5 // 5
#define RIGHT_FORWARD                                  PD6 // 6
#define RIGHT_BACKWORD                                 PD7 // 7

/* define sensors PIN's  */
// PINC_reg
#define IR0                                            PC0
#define IR1                                            PC1
#define IR2                                            PC2
#define IR3                                            PC3
#define IR4                                            PC4


#define clp                                            9


/*IR's bits value*/
 byte sensors_reading = 0b11111;  // to read the first six bits from port_c ..(Front_sensors)
 byte sensor_matrix[5] = {};

 /**/
 boolean noLineFlag = false;

 /**/
 byte clpVal = 0;

/* Functions */


void set_ir_pins()
{
	DDRC &= ~(1 << IR0) & ~(1 << IR1) & ~(1 << IR2) & ~(1 << IR3) & ~(1 << IR4);

	PORTC |= (1 << IR0) | (1 << IR1) | (1 << IR2) | (1 << IR3) | (1 << IR4);

	pinMode(clp, OUTPUT);

}

void read_sensors()
{
	clpVal = digitalRead(clp);
	sensors_reading = 0b11111 & ~PINC;
}

void read_sensor_matrix()
{
	sensor_matrix[0] = digitalRead(A0);
	sensor_matrix[1] = digitalRead(A1);
	sensor_matrix[2] = digitalRead(A2);
	sensor_matrix[3] = digitalRead(A3);
	sensor_matrix[4] = digitalRead(A4);
}

float calculate_error()
{
	static float iError = 0;
	static float prevIerror = 0;
	byte cError = 0; // number of the on-the-line sensors;

	for (size_t i = 0; i < 5; i++)
	{
		if (sensor_matrix[i] == 0)
			cError++;
	}

	if (cError == 0)
	{
		noLineFlag = true;
		iError = prevIerror;

	}
	else
	{
		noLineFlag = false;
		iError = -(((sensor_matrix[0] * (-5)) + (sensor_matrix[1] * (-2)) + (sensor_matrix[2] * (0)) + (sensor_matrix[3] * (2)) + (sensor_matrix[4] * (5))) / cError);
	}

	return iError;

	prevIerror = iError;
}

void initPcPwm_2(void)
{
	/*    Fpwm = (Fcpu / (N*510)) >> N : prescaler  */
	TCCR2A = 0;
	TCCR2B = 0;
	TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20);
	TCCR2B |= (1 << CS20) | (1 << CS21);  //  /32 >> 2us at pulse>> Fpwm = 980.4 Hz
}


void initH_bridge(void)
{
	DDRD = (1 << LEFT_FORWARD) | (1 << LEFT_BACKWORD) | (1 << RIGHT_FORWARD) | (1 << RIGHT_BACKWORD) | (1 << LEFT_MOTOR);
	DDRB = (1 << RIGHT_MOTOR);
}

void setLeftSpeed(int dcSpeed)
{
	boolean reverse = 0;
	if (dcSpeed < 0)
	{
		dcSpeed = -dcSpeed;
		reverse = 1;
	}

	if (dcSpeed > 255)
	{
		dcSpeed = 255;
	}


	if (reverse)
	{
		PORTD |= (1 << LEFT_BACKWORD);
		PORTD &= ~(1 << LEFT_FORWARD);
	}

	else
	{
		PORTD |= (1 << LEFT_FORWARD);
		PORTD &= ~(1 << LEFT_BACKWORD);
	}

	OCR2B = dcSpeed;

}


void setRightSpeed(int dcSpeed)
{
	boolean reverse = 0;
	if (dcSpeed < 0)
	{
		dcSpeed = -dcSpeed;
		reverse = 1;
	}
	if (dcSpeed > 255)
	{
		dcSpeed = 255;
	}


	if (reverse)
	{
		PORTD |= (1 << RIGHT_BACKWORD);
		PORTD &= ~(1 << RIGHT_FORWARD);
	}
	else
	{
		PORTD |= (1 << RIGHT_FORWARD);
		PORTD &= ~(1 << RIGHT_BACKWORD);
	}

	OCR2A = dcSpeed;
}

void setSpeeds(int leftSpeed, int rightSpeed)
{
	setLeftSpeed(leftSpeed);
	setRightSpeed(rightSpeed);
}

void stop_motors()
{
	PORTD &= ~(1 << RIGHT_FORWARD) & ~(1 << RIGHT_BACKWORD) & ~(1 << LEFT_FORWARD) & ~(1 << LEFT_BACKWORD);
}