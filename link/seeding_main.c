// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"

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

int main()
{
	currstate = INIT;
	state(INIT){
		//light_start(L_SENSOR);		// light start
		next(BOX_DRIVE);
	}
	state(BOX_DRIVE){
		forward(40.00);				// forward for 40 cm	
		left(97, ks/2);				// left 90 degrees (97 because the function undershoots)
		forward(20.00);				// forward 20 cm
		//measure to get to box
		next(TRIBBLE_ONE);
	}
	state(TRIBBLE_ONE){
		next(CROSS);
	}
	state(CROSS){
		next(TRIBBLE_TWO);
	}
	state(TRIBBLE_TWO){
		next(SCORE);
	}
	state(SCORE){
		next(END);
	}
	state(END){
		printf("finished");
		ao();
	}
}
