// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"

#define MAIN
//#define COLOR_TEST

/*
*	TODO:
*	1. Test RUN!!!
*	2. ASK LAUREN FOR CODE
*	3. 
*	4. 
*	
*	Important Measurements and Positions:
*	Robot length (wheel guard to front)- 35 cm
*	Robot width (left to right)- 18 cm
*	Gate open- 213
*	Gate close- 1760
*/

//state system
#define s_INIT 100								// initialize
#define s_TRIBBLE_ONE 300						// get the first tribble pile, sort and score
#define s_TRIBBLE_TWO 200 						// get the second tribble pile
#define s_CROSS 400								// cross the field
#define s_TRIBBLE_THREE 500						// get the third tribble pile
#define s_TRIBBLE_FOUR 800 						// get the fourth tribble pile
#define s_CROSSOVER 900							// cross back to the other side of the field
#define s_SCORE 600								// come back and score
#define s_END 700								// finish

#define L_SENSOR 0 								// add light sensor port later
#define TOPHAT1 0
#define TOPHAT2 1
#define threshold 512							// threshold for tophat sensor

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 1								// gate servo ports
#define S_CATCHER 2

#define S_OPEN 200								// servo open/closed positions
#define S_CLOSE 1760

#define S_UP 213								// servo open/closed positions
#define S_DOWN 1760

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
		enable_servos();						// enable servos
		//light_start(L_SENSOR);				// light start
		next(s_TRIBBLE_ONE);
	}
	
	state(s_TRIBBLE_ONE)
	{
		right(45,ks/2);							// right 40 degrees (47 because the function undershoots)
		msleep(100);
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(56.00);							// forward 56 cm
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		// add forward until seeing black then white, then turn left until see black
		
		printf("tribble 1 achieved");
		next(s_TRIBBLE_TWO);
	}
	
	state(s_TRIBBLE_TWO)
	{
		left(47,ks/2);							// return to line
		forward(100.00);						// forward 100 cm
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(25.00);							// forward 25 cm
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 2 achieved");
		next(s_CROSS);
	}
	
	state(s_CROSS)
	{
		backward(60.00);						// backward 60 cm to middle of the board 
		right(97,0);							// right 90 degrees
		forward(95.00);   						// forward 95 cm to cross the little bridge
		printf("crossed over to other side");
		next(s_TRIBBLE_THREE);
	}
	
	state(s_TRIBBLE_THREE)
	{
		right(97,ks/2);							// right 90 degrees
		forward(60.00);							// forward 60 cm to prepare to get tribble
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(21.00);							// forward 21 to collect tribbles
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 3 achieved");
		next(s_TRIBBLE_FOUR);
	}
	
	state(s_TRIBBLE_FOUR)
	{
		backward(90.00);						// backward 90 cm
		right(150,0);							// right 150 degrees
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(35.00);							// forward to collect tribbles
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 4 achieved");
		next(s_CROSSOVER);
	}
	
	state(s_CROSSOVER)
	{
		right(230,ks/2);						// turn right to align with line
		msleep(10);								
		forward(80.00);							// forward to center of field
		msleep(20);						
		left(90,0);								// turn left to crossover
		msleep(10);
		forward(70.00);							// start going forward to score
		printf("crossed back our side of the field");
		next(s_SCORE);
	}
	
	state(s_SCORE)
	{
		forward(60.00);
		// camera find code here
		// forward() score green tribbles
		// turn 
		next(s_END);
	}
	
	state(s_END)
	{
		printf("finished");
		ao();
	}
}
#endif

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
