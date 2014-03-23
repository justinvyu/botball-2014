// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"

//define main if you want to run the main function, define COLOR_TEST if you want to run that program
// REMEMBER NO MORE THAN ONE MAIN PROGRAM
//#define MAIN
#define COLOR_TEST
/*
	TODO:
	1. Add in Servo Positions
	2. Get Touch Sensors and MOT_TREAD
	3. Update drive.h
	4. Everyone get a GITHUB
	5. Measure and code
*/

//state system
#define INIT 100 //initialize
#define BOX_DRIVE 200	//drive and get the box, place the box to score
#define TRIBBLE_ONE 300		//get the first tribble pile, sort and score
#define CROSS 400	//cross the field
#define TRIBBLE_TWO 500		//get the second tribble pile
#define SCORE 600	//come back and score
#define END 700		//finish

#define L_SENSOR 0 //add light sensor port later

#define COL_GREEN 0 //camera channels green/pink
#define COL_PINK 1

//  DEFINE THISE 

//#define MOT_TREAD 2
//#define TOUCH_LEFT
//#define TOUCH_RIGHT

/* Add In Positions     
*	SERVO POSITIONS :   
*		TOP				
*		BOTTOM  		
*						
*/		

void init()
{
	//add more stuff if need to do moar
	camera_open(MED_RES);
}

int get_object_count_both(int ch1, int ch2){
	if(get_object_count(ch1) < 1 && get_object_count(ch2) < 1){
		return 0;
	}
	return 1;
}

#ifdef MAIN
int main()
{
	currstate = INIT;
	state(INIT)
	{
		//light_start(L_SENSOR);		// light start
		next(BOX_DRIVE);
	}
	state(BOX_DRIVE)
	{
		forward(40.00);				// forward for 40 cm	
		left(97, ks/2);				// left 90 degrees (97 because the function undershoots)
		forward(20.00);				// forward 20 cm
		//measure to get to box
		//finish by 3/22/14
		next(TRIBBLE_ONE);
	}
	state(TRIBBLE_ONE)
	{
		//finish by 3/22/14
		next(CROSS);
	}
	state(CROSS)
	{
		//finish by 3/28/14
		next(TRIBBLE_TWO);
	}
	state(TRIBBLE_TWO)
	{
		//finish by 3/28/14
		next(SCORE);
	}
	state(SCORE)
	{
		//finish by 3/28/14
		next(END);
	}
	state(END)
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
		while(get_object_count_both(COL_GREEN, COL_PINK) == 0){
			//mav(MOT_TREAD, 700);
			//msleep(20);
			printf("nothing");
		}
		if(cam_area(COL_GREEN) > 50) //gets area of blob if present
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
		}
	}
}
#endif

