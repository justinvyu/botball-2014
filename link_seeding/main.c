// Created on Sat March 22 2014

#include "generic.h"
#include "drive.h"
#include "slowservo.h"

#define MAIN
//#define COLOR_TEST
//#define CAMERA

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
#define TOPHAT_LEFT 1
#define TOPHAT_RIGHT 0
#define TOUCH_SEN 15
//#define threshold 810							// threshold for tophat sensor
#define spd 800

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 3								// gate servo ports
#define S_CATCHER 2

#define S_OPEN 200								// servo open/closed positions
#define S_CLOSE 1760
#define S_UP 1700								// servo open/closed positions
#define S_DOWN 100

//#include "2014_LACT_drive.h"

#define CATCHER 0
#define HALFWAY 800

#define XCOORDINATE 77
#define YCOORDINATE 63
#define THRESHOLD 10

#define HIGH 100
#define LOW -10


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

void f_until_white(int sen)
{
	while(analog10(sen)>BLACK_SEN_THRESH)
	{
		fd(MOT_RIGHT);
		fd(MOT_LEFT);
	}
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
		//light_start(L_SENSOR);				// light start
		shut_down_in(119);
		camera_open(LOW_RES);
		enable_servos();						// enable servos
		set_servo_position(S_CATCHER,S_UP);
		msleep(500);
		set_servo_position(S_GATE,S_CLOSE);
		msleep(500);
		next(s_TRIBBLE_ONE);
	}
	
	state(s_TRIBBLE_ONE)
	{
		right(45,ks/2);							// right 40 degrees (47 because the function undershoots)
		msleep(100);
		set_servo_position(S_GATE,S_OPEN);		// open gate
		f_until_black(TOPHAT_RIGHT);			// forward until right sensor sees black
		printf("see black! time to close the gate\n");
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		f_until_white(TOPHAT_RIGHT);
		printf("tribble 1 achieved\n");
		next(s_TRIBBLE_TWO);
	}
	
	state(s_TRIBBLE_TWO)
	{
		left(40,ks/2);							// return to line
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
		left(180,0);
		//add a square back, add touch sensor
		while(digital(TOUCH_SEN) != 1)
		{
			bk(MOT_RIGHT);
			bk(MOT_LEFT);
		}
		backward(5.00);
		forward_slow(85.00);					// forward  cm to middle of the board 
		left(90,ks);							// right 90 degrees
     	forward_slow(55.00);   					// forward 95 cm to cross the little bridge
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

#ifdef CAMERA

// Start up the camera and specify the resolution
enable_servo(S_CATCHER); //enables pickup
pickup(S_UP);
camera_open();	
int x, y, color=0;  // set up for color channel 0 (green)
int xvalue, yvalue;
int greencentered = 0;
while (greencentered == 0)
{
	camera_update(); // process the most recent image
	if (get_object_count(color) > 0)
	{
		xvalue = get_object_center(color,0).x;
		yvalue = get_object_center(color,0).y;
		greencentered = XCOORDINATE-THRESHOLD<xvalue<XCOORDINATE+THRESHOLD && YCOORDINATE-THRESHOLD<yvalue<YCOORDINATE+THRESHOLD;
		if(greencentered == 1)//get x, y for the biggest blob the channel sees		
		{	
			printf("Biggest blob at (%i,%i)\n",x,y);
			msleep(200);
			catcher(S_DOWN);
			sleep(1);
			forward(0.6);
			backward(0.6);
			catcher(S_UP);
			msleep(200);
		}
		else
		{
			if(xvalue>87) //moves left if senses value greater than 65
			{motor(MOT_LEFT,10);}
			
			if(xvalue<67) // moves right if senses value less than 55
			{motor(MOT_RIGHT,10);}
			
			if(67<xvalue<87)
			{
				if(yvalue<53) //moves forward if senses value greater than 43
				{
					motor(MOT_LEFT,10);
					motor(MOT_RIGHT,10);
				}
				
				if(yvalue>73) // moves backwar if senses value less than 38
				{
					motor(MOT_LEFT,-10);
					motor(MOT_RIGHT,-10);
				}
			}
		}
	}
}
#endif
