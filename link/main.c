// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"

#define MAIN
//#define COLOR_TEST
/*
	TODO:
	1. Add in Servo Positions
	2. Get Touch Sensors and MOT_TREAD
	3. Update drive.h
	4. Everyone get a GITHUB
	5. Measure and code
	
	Important Measurements and Positions:
	Robot length (wheel guard to front)- 35 cm
	Robot width (left to right)- 18 cm
	Gate open- 213
	Gate close- 1760
*/

//state system
#define s_INIT 100	//initialize
#define s_TRIBBLE_ONE 300	//get the first tribble pile, sort and score
#define s_TRIBBLE_TWO 200 //get the second tribble pile
#define s_CROSS 400		//cross the field
#define s_TRIBBLE_THREE 500	//get the third tribble pile
#define s_TRIBBLE_THREE 800 //get the fourth tribble pile
#define s_SCORE 600		//come back and score
#define s_END 700	//finish

#define L_SENSOR 0 //add light sensor port later

#define COL_GREEN 0 //camera channels green/pink
#define COL_PINK 1

#define S_GATE 0
#define S_CATCHER 2

#define S_OPEN 213
#define s_CLOSE 1760

//  DEFINE THISE 

//#define MOT_TREAD 2
//#define TOUCH_LEFT
//#define TOUCH_RIGHT

/* Add In Positions     
*	S_GATE POSITIONS :   
*		OPEN		213		
*		CLOSED  	1760	
*						
*/		

#ifdef MAIN
int main()
{
	currstate = s_INIT;
	state(s_INIT)
	{
		camera_open(LOW_RES);
		enable_servos();				// enable servos
		//light_start(L_SENSOR);		// light start
		next(s_BOX_DRIVE);
	}
	
	state(s_TRIBBLE_ONE)
	{
		right(47,ks/2);		// left 40 degrees (47 because the function undershoots)
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(56.00);		// forward 56 cm
		set_servo_position(s_GATE,S_CLOSE);	// close gate
		next(s_TRIBBLE_TWO);
	}
	
	state(s_TRIBBLE_TWO)
	{
		left(47,ks/2);		//return to line
		forward(100.00);	//forward 100 cm
		set_servo_position(S_GATE,S_OPEN);
		forward(25.00);		//forward 25 cm
		set_servo_position(S_GATE,S_CLOSE);
		next(s_CROSS);
	}
	
	state(s_CROSS)
	{
		forward(60.00);		// forward 60 cm to 
		right(97,ks/2);		// right 90 degrees
		forward(95.00);   	// forward 95 cm to cross the little bridge
		next(s_TRIBBLE_THREE);
	}
	
	state(s_TRIBBLE_TWO)
	{
		//finish by 3/28/14
		next(s_SCORE);
	}
	
	state(s_SCORE)
	{
		//finish by 3/28/14
		next(s_END);
	}
	
	state(s_END)
	{
		printf("finished");
		ao();
	}
}
#endif

// Created on Fri March 21 2014

//drive, generic functions

#ifdef COLOR_TEST
int main()
{
	init();
	
	while(1)
	{
		//spin tread until see a tribble
		if(get_object_count(COL_GREEN) < 1 && get_object_count(COL_PINK) < 1)){
			//mav(MOT_TREAD, 700);
			//msleep(20);
			printf("nothing");
		}
		else if(cam_area(COL_GREEN) > 50) //gets area of blob if present
		{
			//if green, push into green basket
			printf("green");
		}
		else if(cam_area(COL_PINK) > 50)
		{
			//if pink, push into pink basket
			printf("pink");
		}
		else
		{
			camera_update();
			msleep(100);
		}
	}
}
#endif

