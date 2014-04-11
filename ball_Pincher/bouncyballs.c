#include "2014_LACT_drive.h"

#define CATCHERARM 2
#define CATCHER_DOWN 50
#define CATCHER_MIDWAY 700
#define CATCHER_UP 1700
#define HALFWAY 800

#define MOT_LEFT 2 //to go left, left motor go back while right motor go forward
#define MOT_RIGHT 3 // identify channel and motors
#define HIGH 100
#define LOW -10

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
#define S_GAP 1450
#define S_UP 1700								// servo open/closed positions
#define S_DOWN 100

#define INCREMENT 25 
#define SLEEP_INCREMENT 100

int main ()
{
	int x, y, color=0;  // set up for color channel 0 (green)
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
	//light_start(L_SENSOR);				// light start
	shut_down_in(119);
	camera_open(LOW_RES);
	enable_servos();						// enable servos
	set_servo_position(S_CATCHER,S_UP);
	msleep(500);
	set_servo_position(S_GATE,S_CLOSE);
	msleep(500);
	set_servo_position(S_GATE,S_OPEN);		// open gate
	right(30,ks/2);							// right 40 degrees (47 because the function undershoots)
	f_until_black(TOPHAT_RIGHT/*,TOPHAT_LEFT*/);	// forward until right sensor sees black
	printf("see black! time to close the gate\n");
	set_servo_position(S_GATE,S_GAP);		// close gate
	//f_until_white(TOPHAT_RIGHT);
	printf("tribble 1 achieved\n");	
	set_servo_position(CATCHERARM, CATCHER_UP);
	backward(4.0);
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
	set_servo_position(S_GATE,S_CLOSE);	
	while (analog10(2)<BLACK_SEN_THRESH)
	{
		motor(MOT_LEFT,-100);
	}
	left(10,0);
	backward(50);
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
	backward(30);
	left(50,0);
	backward(10);
	left(50,0);
	backward(35);
	linefollow(2,20,100,80);
	/*left(30,ks/2);							// return to line
	forward_slow(40);					// forward 100 cm
	linefollow(0,10,100,80);
	forward(20.00);
	set_servo_position(S_GATE,S_OPEN);		// open gate
	forward(70.00);							// forward 25 cm
	set_servo_position(S_GATE,S_CLOSE);		// close gate
	printf("tribble 2 achieved\n");
	left(100,0);
	//backward(20);
	//add a square back, add touch sensor
	/*while(digital(TOUCH_SEN) != 1)
	{
	bk(MOT_RIGHT);
	bk(MOT_LEFT);
	}*/
	//backward(5.00);
	forward(85);					// forward  cm to middle of the board 
	forward_slow(55);   					// forward 95 cm to cross the little bridge
	forward(40.00);
	printf("crossed over to other side\n");
	right(97,ks/2);							// right 90 degrees
	forward(60.00);							// forward 60 cm to prepare to get tribble
	set_servo_position(S_GATE,S_OPEN);		// open gate
	forward(21.00);							// forward 21 to collect tribbles
	set_servo_position(S_GATE,S_CLOSE);		// close gate
	printf("tribble 3 achieved");
	backward(90.00);						// backward 90 cm
	right(150,0);							// right 150 degrees
	set_servo_position(S_GATE,S_OPEN);		// open gate
	forward(35.00);							// forward to collect tribbles
	set_servo_position(S_GATE,S_CLOSE);		// close gate
	printf("tribble 4 achieved\n");
	right(230,ks/2);						// turn right to align with line
	msleep(10);								
	forward(80.00);							// forward to center of field
	msleep(20);						
	left(90,0);								// turn left to crossover
	msleep(10);
	forward(70.00);							// start going forward to score
	printf("crossed back our side of the field\n");
}
