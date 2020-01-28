/*
 Name:		Line_Follower_adb.ino
 Created:	2/26/2019 11:59:09 AM
 Author:	mohei_aldeen
*/


#include <Error.h>
#include <Init.h>   


#define RED_LED     12
#define GREEN_LED   13



float Kp = 13.15, Ki = 0.2, Kd = 450, Ksr = 1.5; //Kp = 20, Ki = 0.01, Kd = 300, Ksr = 3.5; //Kp = 14, Ki = 0.09, Kd = 175, Ksr = 3; //
float error = 0, error_sum_I = 0, error_sum_SR = 0;
uint8_t error_counter = 0;
float P = 0, I = 0, D = 0, PID_value = 0, SR = 0;
float previous_error = 0, previous_I = 0;

boolean no_line_flag = false;
byte previous_case = 0;

// add some timing calculation ...
const int sampling_time = 4000;  //microseconds ..
int one_second = 1000000 / sampling_time;
int one_milli_second = one_second / 1000;
int loop_counter;

unsigned long loop_timer; // for the loop delay ;


int initial_motor_speed = 95;


/*......Functions....*/

/*   PID calculation  */
void calculate_pid()
{
	
	error_counter++;

	error_sum_I += error;
	error_sum_SR += error;

	P = Kp * error;
	I = Ki * error_sum_I;
	D = Kd * (error - previous_error);

	/* the differential and the speed reduction values */
	PID_value = P + I + D;
	SR = Ksr * abs(error_sum_SR);

	if (error_counter >= 10)
	{
		error_counter = 0;
		error_sum_SR = 0;
		error_sum_I *= 0.0; // 1% of the previous error sum .
	}

	previous_error = error;
}

/*  motor control  */
void motor_control()
{
	int left_motor_speed, right_motor_speed;
	loop_counter = 0;

	if (!no_line_flag)
	{
		digitalWrite(GREEN_LED, HIGH);
		digitalWrite(RED_LED, LOW);

		left_motor_speed = ((initial_motor_speed + PID_value) - SR);
		right_motor_speed = ((initial_motor_speed - PID_value) - SR);


		if (left_motor_speed < 40) left_motor_speed = 40;
		if (left_motor_speed > 255) left_motor_speed = 255;

		if (right_motor_speed < 40) right_motor_speed = 40;
		if (right_motor_speed > 255) right_motor_speed = 255;


		setSpeeds(left_motor_speed + 45, right_motor_speed);

	}

	else
	{
		digitalWrite(GREEN_LED, LOW);
		digitalWrite(RED_LED, HIGH);
		loop_counter++;
		left_motor_speed = 0;
		left_motor_speed = 0;
		stop_motors();
		search();
	}

	if (clpVal == 1)
		stop_motors();

	//Serial.print("lef =  "); Serial.println(left_motor_speed);
	//Serial.print("righ =  "); Serial.println(right_motor_speed);
}


/*  search  */
void search()
{
	static boolean go_left;
	static boolean go_right;
	static boolean search_flag;

	if (loop_counter == int(one_second / 2))
	{
		search_flag = true;
		go_left = false;
		go_right = false;
	}

	if (search_flag && loop_counter <= int(one_second*3))
	{
		// turn left ...
		setSpeeds(0, 100);
		read_sensors();
		if (sensors_reading != NO_LINE)
			return;
		if (loop_counter == int(one_second*3))
		{
			go_right = true;
			search_flag = false;
			loop_counter = one_second;
		}
	}

	if (go_right)
	{
		setSpeeds(100, 90);
		read_sensors();
		if (sensors_reading != NO_LINE)
			return;
		if (loop_counter % one_second == 0)
		{
			go_right = false;
			go_left = true;
			loop_counter++;
		}
	}

	if (go_left)
	{
		setSpeeds(80, 110);
		read_sensors();
		if (sensors_reading != NO_LINE)
			return;
		if (loop_counter % one_second == 0)
		{
			go_right = true;
			go_left = false;
			loop_counter++;
		}
	}

	read_sensors();

	if (sensors_reading != NO_LINE)
		return;
}


void setup()
{
	Serial.begin(9600);
	set_ir_pins();
	initPcPwm_2();
	initH_bridge();
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	loop_timer = micros() + sampling_time;
}

void loop()
{

	read_sensors();  // 4uS

	set_error();

	calculate_pid();

	motor_control();

	//setSpeeds(255, 255);


	while (loop_timer > micros());
	loop_timer += sampling_time;
}



void set_error()
{
	static boolean incidental_no_line = false;

	static int no_line_timer = 0;
	static float incidental_noline_timer = 0;
	static float someTime = one_second * 3;

	static float internal_previous_error;

	switch (sensors_reading)
	{

		/*handle all the sensors cases*/

	case OFF_RIGHT:
		error = OFF_RIGHT_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case REAL_RIGHT:
		error = REAL_RIGHT_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;


	case RIGHT_STRAIGHT_INTERSECTION:
		error = RIGHT_STRAIGHT_INTERSECTION_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case RIGHT_NEAR_CENTER:
		error = RIGHTT_NEAR_CENTER_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;


	case CENTER:
		error = CENTER_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case T_INTERSECTION:
		error = T_INTERSECTION_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case CROSS_INTERSECTION:
		error = CROSS_INTERSECTION_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case LEFT_NEAR_CENTER:
		error = LEFT_NEAR_CENTER_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case LEFT_STRAIGHT_INTERSECTION:
		error += LEFT_STRAIGHT_INTERSECTION_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

	case REAL_LEFT:
		error = REAL_LEFT_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;


	case OFF_LEFT:
		error = OFF_LEFT_ERROR;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;

		/*handle the special no_line case*/
	case NO_LINE:
		no_line_timer++;
		if (no_line_timer > (someTime))
		{
			no_line_flag = true;
			incidental_no_line = false;
		}

		// handle the incidental no line state ...
		if (!no_line_flag)
		{

			incidental_noline_timer++;
			incidental_no_line = true;

			if (incidental_no_line)
			{
				error = internal_previous_error;

				if (incidental_noline_timer > ((someTime)-1))
				{
					incidental_no_line = false;
					incidental_noline_timer = 0;
				}
			}
			
		}

		/*else
		{
		handle the no_line case that take longer time in the motor control function ..
		}
		*/
		break;


		// any case else ... "default"
	default:
		error = internal_previous_error;
		no_line_flag = false;
		no_line_timer = 0;
		incidental_noline_timer = 0;
		break;
	}


	if (sensors_reading != NO_LINE)
		previous_case = sensors_reading;

	internal_previous_error = error;

}
