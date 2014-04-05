// Created on Fri March 28 2014

#include "drive.h"
#include "generic.h"

//define main for main()
#define MAIN

//state system
#define s_INIT 100								// initialize
#define s_BLOCKADE 400							// put down the blockade at the bridge
#define s_TRIBBLE_ONE 300						// get the first tribble pile
#define s_TRIBBLE_TWO 200 						// get the second tribble pile
#define s_BLUE_CUBE 700							// get the blue cube and score... i think
#define s_SCORE 500								// points
#define s_END 600								// end

#define L_SENSOR 0 								// add light sensor port later

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 0								// gate servo positions
#define S_CATCHER 2

#define S_OPEN 213								// servo open/closed positions
#define S_OPEN_PART 600
#define s_CLOSE 1760

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
		camera_open(LOW_RES);
		enable_servos();						// enable servos
		set_servo_position(S_CATCHER,S_UP);
		msleep(500);
		set_servo_position(S_GATE,S_CLOSE);
		msleep(500);
		//light_start(L_SENSOR);				// light start
		next(s_TRIBBLE_ONE);
	}
	
	/*state(s_BLOCKADE)
	{
		next(s_TRIBBLE_ONE);
	}*/
	
	state(s_TRIBBLE_ONE)
	{
		right(50,ks/2);							// right 40 degrees (47 because the function undershoots)
		msleep(100);
		set_servo_position(S_GATE,S_OPEN);		// open gate
		f_until_black(TOPHAT_RIGHT);			// forward until right sensor sees black
		printf("see black! time to close the gate\n");
		set_servo_position(S_GATE,S_CLOSE);		// close gate
		printf("tribble 1 achieved\n");
		next(s_BLUE_CUBE);
	}
	
	state(s_BLUE_CUBE) //decide whether or not to do this
	{
		f_until_white(TOPHAT_RIGHT);
		left(33,ks/2);
		forward_slow(60);
		forward(20);
		left(42,ks/2);
		forward(58);
		right(42,ks/2);
		set_servo_position(S_GATE,S_OPEN_PART);
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
		next(s_SCORE);
	}
	
	state(s_SCORE)
	{
		next(s_END);
	}
	
	state(s_END)
	{
		
	}
}
#endif
