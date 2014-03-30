void servo_off(int servo)
{ set_servo_position(servo,-1); }

void servo_slow(int port, int end, float time)
{
	float increment = .01;
	float curr,start = get_servo_position(port);
	float i = ((end-start)/time)*increment;
	curr = start;

	if (start > end)
	{
		while(curr > end)
		{
			//printf("%f\n",curr);
			set_servo_position(port,curr);
			curr+=i;
			sleep(increment);
		}
	}
	else if (start < end)
	{
		while(curr < end)
		{
			//printf("%f\n",curr);
			set_servo_position(port,curr);
			curr+=i;
			sleep(increment);
		}
	}
	set_servo_position(port,end);
}// Created on Sat March 29 2014

// Replace FILE with your file's name
#ifndef _FILE_H_
#define _FILE_H_



#endif
