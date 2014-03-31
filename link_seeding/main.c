// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"
#include "slowservo.h"

#define MAIN
//#define COLOR_TEST
//#define TEST

/*
*	TODO:
*	1. Start to test run, add in code starting from line 166
*	2. Ask lauren for help on camera code in S_SCORE
*	3. After you finish and "round out" the code, please work on main.c in Link_DE
*	4. Finish link_DE main.c
*	
*	Important Measurements and Positions:
*	Robot length (wheel guard to front)- 35 cm
*	Robot width (left to right)- 18 cm
*	Gate open- 213w
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
#define TOPHAT_LEFT 1
#define TOPHAT_RIGHT 0
#define threshold 810							// threshold for tophat sensor
#define spd 800

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 3								// gate servo ports
#define S_CATCHER 2

#define S_OPEN 200								// servo open/closed positions
#define S_CLOSE 1760
#define S_UP 1700								// servo open/closed positions
#define S_DOWN 100

//  DEFINE THISE 

//#define MOT_TREAD 2
//#define TOUCH_LEFT
//#define TOUCH_RIGHT

/* Add In Positions     
*	S_GATE POSITIONS :   
*		OPEN		213		
*		CLOSED  	1760	
*	
*	S_CATCHER positions :
*		UP			2047
*		DOWN		100
*/		

void catch()
{
	set_servo_position(S_CATCHER,S_DOWN);
	msleep(1000);
	//printf("down");
	set_servo_position(S_CATCHER,S_UP);
	msleep(1000);
	//printf("up");
}

void f_until_black(int sen)
{
	while(analog10(sen)<BLACK_SEN_THRESH)
	{
		fd(MOT_RIGHT);
		fd(MOT_LEFT);
	}
}

void l_until_black(int sen)
{
	while(analog10(sen)<BLACK_SEN_THRESH)
	{
		mav(MOT_RIGHT,spd);
		msleep(1);
	}
}

void r_until_black(int sen)
{
	while(analog10(sen)<BLACK_SEN_THRESH)
	{
		mav(MOT_LEFT,spd);
		msleep(1);
	}
}

void forward_slow(int distance)
{
	if(distance < 0l){
		distance = -distance;
	}
	long newdist;
	newdist = distance*CMtoBEMF;//conversion ratio
	mrp(MOT_RIGHT,500,newdist*rdistmult);
	mrp(MOT_LEFT,500,newdist);
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
}

#ifdef MAIN
int main()
{
	currstate = s_INIT;
	state(s_INIT)
	{
		camera_open(LOW_RES);
		enable_servos();						// enable servos
		set_servo_position(S_CATCHER,S_UP);
		msleep(500);
		set_servo_position(S_GATE,S_CLOSE);
		msleep(500);
		//light_start(L_SENSOR);				// light start
		next(s_TRIBBLE_ONE);
	}
	
	state(s_TRIBBLE_ONE)
	{
		right(50,ks/2);							// right 40 degrees (47 because the function undershoots)
		msleep(100);
		set_servo_position(S_GATE,S_OPEN);		// open gate
		f_until_black(TOPHAT_RIGHT);			// forward until right sensor sees black
		printf("see black! time to close the gate\n");
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 1 achieved\n");
		next(s_TRIBBLE_TWO);
	}
	
	state(s_TRIBBLE_TWO)
	{
		left(38,ks/2);							// return to line
		forward_slow(40.00);						// forward 100 cm
		forward(20.00);
		set_servo_position(S_GATE,S_OPEN);		// open gate
		forward(42.00);							// forward 25 cm
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 2 achieved\n");
		next(s_CROSS);
	}
	
	state(s_CROSS)
	{
		right(180,0);
		forward(32.00);						// backward 60 cm to middle of the board 
		left(90,0);							// right 90 degrees
		//WORKS UP TO HERE
		//PLEASE TEST AND FIX THE CODE BELOW, USE THE F_UNTIL_BLACK and L_UNTIL_BLACK or R_UNTIL_BLACK FOR CONSTISTENCY
/*-->*/	forward_slow(55.00);   						// forward 95 cm to cross the little bridge
		forward(40.00);
		printf("crossed over to other side\n");
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
		printf("tribble 4 achieved\n");
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
		printf("crossed back our side of the field\n");
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
		printf("finished\n");
		ao();
		disable_servos();
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

#ifdef TEST
int main()
{
	forward(80);
	msleep(100);
}
#endif
