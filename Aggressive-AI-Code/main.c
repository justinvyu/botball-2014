// Created on Fri March 28 2014

#include "2014_LACT_drive.h"

#define MAIN
//#define TEST

#define CATCHERARM 2
#define CATCHER_DOWN 50
#define CATCHER_MIDWAY 700
#define CATCHER_UP 1700
#define HALFWAY 800

#define HIGH 100
#define LOW -10

#define L_SENSOR 4 								// add light sensor port later
#define TOPHAT_LEFT 1
#define TOPHAT_RIGHT 0
#define TOUCH_SEN 15
#define spd 800

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 3								// gate servo ports
#define S_CATCHER 2

#define S_OPEN 200								// servo open/closed positions
#define S_CLOSE 1760
#define S_UP 1700								// servo up/down positions
#define S_DOWN 100
#define S_GAP 1450

#define INCREMENT 25 
#define SLEEP_INCREMENT 100

void touch_back(int sen)
{
	while(digital(TOUCH_SEN) == 0)
	{
		bk(MOT_RIGHT);
		bk(MOT_LEFT);
	}
}

#ifdef MAIN
int main()
{
	// INITIALIZE
	
	int x, y, color=0, pink=1;  // set up for color channel 0 (green)
	int xvalue, yvalue;
	int greencentered = 0;
	int loop = 0;
	int centerx=77; // calibrate these number and enter manually
	int centery=63;
	int margin=8;
	int deltax = 0;
	int deltay = 0;
	int stepsizex;
	int stepsizey;
	int currpos;
	int s_time;
	
	light_start(L_SENSOR);				// light start
	shut_down_in(119);
	camera_open(LOW_RES);
	enable_servos();						// enable servos
	set_servo_position(S_CATCHER,S_DOWN);
	msleep(500);
	set_servo_position(S_GATE,S_OPEN);
	msleep(500);
	
	// TRIBBLE PILE 1
	
	//set_servo_position(S_GATE,S_OPEN);		// open gate
	msleep(5000);
	set_servo_position(S_CATCHER,CATCHER_UP);
	right(28,ks/2);									// right
	f_until_black(TOPHAT_RIGHT/*,TOPHAT_LEFT*/);	// forward until right sensor sees black]
	forward(2);
	//printf("see black! time to close the gate\n");
	set_servo_position(S_GATE,S_GAP);				// close gate
	//f_until_white(TOPHAT_RIGHT);
	set_servo_position(CATCHERARM, CATCHER_UP);
	
	// camera sort

	s_time = curr_time();
	while((curr_time()-s_time) < 14)
	{
		while (greencentered == 0)
		{
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			loop = loop + 1;
			printf("Update camera, loop %d\n", loop);
			//msleep(10);
			if (get_object_count(color) > 0)
			{
				xvalue = get_object_center(color,0).x;
				yvalue = get_object_center(color,0).y;
				deltax = abs(xvalue-centerx);
				deltay = abs(yvalue-centery);
				stepsizex = (deltax>10) ? 50: 20;
				stepsizey = (deltay>10) ? 50: 20;
				printf("x is %d, y is %d\n", xvalue, yvalue);
				greencentered = ((xvalue >= centerx-margin) & (xvalue <= centerx+margin)) && ((yvalue >= centery-margin) & (yvalue <= centery+margin));
				if(greencentered == 1)//get x, y for the biggest blob the channel sees		
				{	
					printf("Biggest blob at (%d,%d)\n",xvalue,yvalue);
					msleep(300);
					set_servo_position(CATCHERARM, CATCHER_MIDWAY);
					msleep(300);
					set_servo_position(CATCHERARM, CATCHER_DOWN);
					//slow_servo(CATCHERARM,20,40,CATCHER_UP,CATCHER_DOWN);
					msleep(300);
					currpos=get_servo_position(CATCHERARM);
					printf("%d\n",currpos);
					/*if(CATCHER_MIDWAY>=currpos>CATCHER_DOWN)
					{	
					currpos=get_servo_position(CATCHERARM);
					set_servo_position(CATCHERARM, currpos-INCREMENT);
					msleep(SLEEP_INCREMENT);
					currpos=get_servo_position(CATCHERARM);
					}
					if(currpos<CATCHER_MIDWAY)
					{	
					currpos=get_servo_position(CATCHERARM);
					set_servo_position(CATCHERARM, currpos+INCREMENT);
					msleep(SLEEP_INCREMENT);
					currpos=get_servo_position(CATCHERARM);
					}*/
					set_servo_position(CATCHERARM,CATCHER_UP);
					msleep(500);
				}
				else
				{
					if(xvalue <= centerx+margin) //moves right if senses value greater than 80
					{
						motor(MOT_RIGHT,70);
						motor(MOT_LEFT,-70);
						msleep(stepsizex);
						motor(MOT_RIGHT,0);
						motor(MOT_LEFT,0);
						//msleep(10);
						printf("blob is too right\n");
					}	
					if(xvalue >= centerx-margin) // moves left if senses value less than 70
					{
						motor(MOT_LEFT,70);
						motor(MOT_RIGHT,-70);
						msleep(stepsizex);	
						motor(MOT_LEFT,0);
						motor(MOT_RIGHT,0);
						//msleep(10);
						printf("blob is too left\n");
					}
					
					if ((xvalue > 67 /*centerx-margin*/) && (xvalue < 87 /*centerx+margin*/))
					{
						if(yvalue > centery+margin) //moves forward if senses value greater than 43
						{
							motor(MOT_LEFT,-50);
							motor(MOT_RIGHT,-50);
							msleep(stepsizey);
							motor(MOT_LEFT,0);
							motor(MOT_RIGHT,0);
							//msleep(10);
							printf("blob is too close\n");
						}
						
						if(yvalue < centery-margin) // moves backwar if senses value less than 38
						{
							motor(MOT_LEFT,50);
							motor(MOT_RIGHT,50);
							msleep(stepsizey);
							motor(MOT_LEFT,0);
							motor(MOT_RIGHT,0);
							//msleep(10);
							printf("blob is too far\n");
						}
					}
				}
			}
		}
	}
	set_servo_position(S_GATE,S_CLOSE);	
	while (analog10(2)<BLACK_SEN_THRESH)
	{
		motor(MOT_LEFT,-100);
	}
	
	// SCORE PILE 1
	
	left(5,0);
	//backward(50);
	touch_back(TOUCH_SEN);
	forward(10);
	backward(10);
	forward(20);
	right(55,0);
	forward(35);
	backward(4);
	right(45,0);
	while (analog10(3)<BLACK_SEN_THRESH)
	{
		motor(MOT_LEFT, 100);
		motor(MOT_RIGHT, 100);
	}
	msleep(10);
	backward(24);
	left(42,0);
	backward(10); 
	left(50,0);
	//backward(35);
	touch_back(TOUCH_SEN);
	forward(120.00);
	
	// TRIBBLE PILE 2
	
	set_servo_position(S_GATE,S_OPEN);
	forward(40.00);
	set_servo_position(S_GATE,S_GAP);
	
	// camera sort
	
	s_time = curr_time();
	while((curr_time()-s_time) < 14) //timer
	{
		while (greencentered == 0)
		{
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			camera_update(); // process the most recent image
			loop = loop + 1;
			printf("Update camera, loop %d\n", loop);
			//msleep(10);
			if (get_object_count(color) > 0)
			{
				xvalue = get_object_center(color,0).x;
				yvalue = get_object_center(color,0).y;
				deltax = abs(xvalue-centerx);
				deltay = abs(yvalue-centery);
				stepsizex = (deltax>10) ? 50: 20;
				stepsizey = (deltay>10) ? 50: 20;
				printf("x is %d, y is %d\n", xvalue, yvalue);
				greencentered = ((xvalue >= centerx-margin) & (xvalue <= centerx+margin)) && ((yvalue >= centery-margin) & (yvalue <= centery+margin));
				if(greencentered == 1)//get x, y for the biggest blob the channel sees		
				{	
					printf("Biggest blob at (%d,%d)\n",xvalue,yvalue);
					msleep(300);
					set_servo_position(CATCHERARM, CATCHER_MIDWAY);
					msleep(300);
					set_servo_position(CATCHERARM, CATCHER_DOWN);
					//slow_servo(CATCHERARM,20,40,CATCHER_UP,CATCHER_DOWN);
					msleep(300);
					currpos=get_servo_position(CATCHERARM);
					printf("%d\n",currpos);
					/*if(CATCHER_MIDWAY>=currpos>CATCHER_DOWN)
					{	
					currpos=get_servo_position(CATCHERARM);
					set_servo_position(CATCHERARM, currpos-INCREMENT);
					msleep(SLEEP_INCREMENT);
					currpos=get_servo_position(CATCHERARM);
					}
					if(currpos<CATCHER_MIDWAY)
					{	
					currpos=get_servo_position(CATCHERARM);
					set_servo_position(CATCHERARM, currpos+INCREMENT);
					msleep(SLEEP_INCREMENT);
					currpos=get_servo_position(CATCHERARM);
					}*/
					set_servo_position(CATCHERARM,CATCHER_UP);
					msleep(500);
				}
				else
				{
					if(xvalue <= centerx+margin) //moves right if senses value greater than 80
					{
						motor(MOT_RIGHT,70);
						motor(MOT_LEFT,-70);
						msleep(stepsizex);
						motor(MOT_RIGHT,0);
						motor(MOT_LEFT,0);
						//msleep(10);
						printf("blob is too right\n");
					}	
					if(xvalue >= centerx-margin) // moves left if senses value less than 70
					{
						motor(MOT_LEFT,70);
						motor(MOT_RIGHT,-70);
						msleep(stepsizex);	
						motor(MOT_LEFT,0);
						motor(MOT_RIGHT,0);
						//msleep(10);
						printf("blob is too left\n");
					}
					
					if ((xvalue > 67 /*centerx-margin*/) && (xvalue < 87 /*centerx+margin*/))
					{
						if(yvalue > centery+margin) //moves forward if senses value greater than 43
						{
							motor(MOT_LEFT,-50);
							motor(MOT_RIGHT,-50);
							msleep(stepsizey);
							motor(MOT_LEFT,0);
							motor(MOT_RIGHT,0);
							//msleep(10);
							printf("blob is too close\n");
						}
						
						if(yvalue < centery-margin) // moves backwar if senses value less than 38
						{
							motor(MOT_LEFT,50);
							motor(MOT_RIGHT,50);
							msleep(stepsizey);
							motor(MOT_LEFT,0);
							motor(MOT_RIGHT,0);
							//msleep(10);
							printf("blob is too far\n");
						}
					}
				}
			}
		}
	}
	set_servo_position(S_GATE,S_CLOSE);
	
	// SCORE PILE 2
	
	right(55,0);
	forward(28);
	backward(12);
	right(55,0);
	touch_back(TOUCH_SEN);
	forward(200);
	right(50,0);
	touch_back(TOUCH_SEN);
	forward(15);
	left(50,0);
	while (analog10(3)<BLACK_SEN_THRESH)
	{
		motor(MOT_LEFT, 100);
		motor(MOT_RIGHT, 100);
	}
	backward(4);
	right(50,0);
	msleep(10);
	forward(24);
	right(42,0);
	touch_back(TOUCH_SEN);
	backward(4);
	
	// END
	ao();
	disable_servos();
	printf("done!");
}
#endif

#ifdef TEST
int main()
{
forward(40);
//f_until_black(TOPHAT_RIGHT);
}
#endif
