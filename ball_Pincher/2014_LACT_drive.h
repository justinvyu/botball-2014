// Created on Fri April 4 2014

#ifndef _2014_LACT_DRIVE_H_
#define _2014_LACT_DRIVE_H_

//primary driving code
float tcc=0.95; 
#define MOT_ARM 2
#define TOUCHSENSOR 15
#define PI 3.14159265358979
#define SPD 1000.
#define SPDnew 500.
#define SPDl 1000.
#define SPDr 1000.
#define rdistmult (SPDr/SPDl)

#define ks 21.//distance from one wheel to another in cm
#define wheeldiameter 6.//actually 4, but added a little for wheel slippage

#define CMtoBEMF (1010/wheeldiameter/PI)
#define LBUMP digital(14)
#define RBUMP digital(15) //left/right back bump sensors


#define drive_off() off(MOT_RIGHT) ;off(MOT_LEFT)
#define drive(mL,mR) mav(MOT_LEFT,mL);mav(MOT_RIGHT,mR)
#define gmpc(mot) get_motor_position_counter(mot)

#define CATCHERARM 2
#define CATCHER_DOWN 50
#define CATCHER_MIDWAY 700
#define CATCHER_UP 1700
#define HALFWAY 800


#define MOT_LEFT 2 //to go left, left motor go back while right motor go forward
#define MOT_RIGHT 3 // identify channel and motors
#define HIGH 100
#define LOW -10

#define BLACK_SEN_THRESH 300
void square_back()
{
	int _A = 0,_B = 0;
	float time = seconds();//used for timeout
	bk(MOT_LEFT);
	bk(MOT_RIGHT);
	while((_A == 0 || _B == 0) && (seconds()-time < 10))//while the bump sensors are false & it has been less than 10 seconds
	// move backwards, if the bot is bumped, turn the motor off, and break out of the loop
	{
		if (LBUMP){//if the left sensor is pressed
			off(MOT_LEFT);//turn towards wall
			_A = 1;
			} else {
			bk(MOT_LEFT);//otherwise, approach the wall more
			_A = 0;
		}
		
		if (RBUMP){//if the right sensor is pressed
			off(MOT_RIGHT);//turn towards wall
			_B = 1;
			} else {
			bk(MOT_RIGHT);//otherwise, approach the wall more
			_B = 0;
		}
		msleep(1);//give other processes time to do things
	}
	drive_off();//turn both motors off at end
}


void right(float degrees, float radius){//turn right a number of degrees with a certain radius
	int turnrspeed;
	long turnl=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnl == 0l) return;
	turnrspeed = round((float)turnr/(float)turnl*SPD);
	mrp(MOT_LEFT, SPD,turnl);
	if(turnrspeed < 0) turnrspeed = -turnrspeed;
	if(turnrspeed < 50){
		turnrspeed = 0;
		turnr = 0l;
		off(MOT_RIGHT);
		}else{
		mrp(MOT_RIGHT,turnrspeed,turnr);
	}
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
}
void left(float degrees, float radius){//turn left a number of degrees with a certain radius
	int turnlspeed;
	long turnl=(long)round((((2*radius-ks)*CMtoBEMF)*PI)*degrees/360.);
	long turnr=(long)round((((2*radius+ks)*CMtoBEMF)*PI)*degrees/360.);
	if(turnr == 0l) return;
	turnlspeed = round((float)turnl/(float)turnr*SPD);
	mrp(MOT_RIGHT, SPD,turnr);
	if(turnlspeed < 0) turnlspeed = -turnlspeed;
	if(turnlspeed < 50){
		turnlspeed = 0;
		turnl=0l;
		off(MOT_LEFT);
		}else{
		mrp(MOT_LEFT,turnlspeed,turnl);
	}
	bmd(MOT_LEFT);
	bmd(MOT_RIGHT);
}
void forward(float distance){//go forward a number of CM NOT backEMF counts
	if(distance < 0l){
		distance = -distance;
	}
	long newdist;
	newdist = distance*CMtoBEMF;//conversion ratio
	mrp(MOT_RIGHT,SPDr*tcc,newdist*rdistmult*tcc);
	mrp(MOT_LEFT,SPDl,newdist);
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
}
void backward(float distance){//go backward a number of CM NOT backEMF counts
	if(distance < 0l){
		distance = -distance;
	}
	long newdist;
	newdist = distance*CMtoBEMF;
	mrp(MOT_RIGHT,SPDr,-newdist*rdistmult);
	mrp(MOT_LEFT,SPDl,-newdist);
	bmd(MOT_RIGHT);
	bmd(MOT_LEFT);
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

void f_until_white(int sen)
{
	while(analog10(sen)>BLACK_SEN_THRESH)
	{
		fd(MOT_RIGHT);
		fd(MOT_LEFT);
	}
}

void f_until_black(int sen/*, int othersen*/)
{
	while(analog10(sen) /*|| analog10(othersen)*/<BLACK_SEN_THRESH)
	{
		motor(MOT_RIGHT,100*tcc);
		motor(MOT_LEFT,100);
	}
	//while(analog10(sen) /*|| analog10(othersen)*/>=BLACK_SEN_THRESH)
	//{
	//	motor(MOT_RIGHT,0);
	//	motor(MOT_LEFT,0);
	//}
}

void l_until_black(int sen)
{
	while(analog10(sen)<BLACK_SEN_THRESH)
	{
		mav(MOT_RIGHT,SPDnew);
		msleep(1);
	}
}

void r_until_black(int sen)
{
	while(analog10(sen)<BLACK_SEN_THRESH)
	{
		mav(MOT_LEFT,SPDnew);
		msleep(1);
	}
}

void linefollow(int sen, int distance_in_inches, int highspeed, int lowspeed) //function to follow line
{
	int i=0;
	int distance=distance_in_inches*250; // turns distance in inches to distance in ticks
	clear_motor_position_counter(MOT_LEFT); //resets motor position counters
	clear_motor_position_counter(MOT_RIGHT);
	while(i<distance) //while the distance has not yet been achieved
	{
		if(analog10(sen)>=BLACK_SEN_THRESH) // if the sensor senses dark
		{
			motor(MOT_LEFT,lowspeed); //then it will turn left
			motor(MOT_RIGHT,highspeed);
			msleep(300); //waits for motors to finish
			i=get_motor_position_counter(1)+get_motor_position_counter(3); // updates distance travelledmav(3,500); 	// then it will turn right
		}
		if(analog10(sen)<BLACK_SEN_THRESH) // if senses white
		{
			
			motor(MOT_RIGHT,lowspeed);
			motor(MOT_LEFT,highspeed);
			msleep(300); // waits for motors to finish
			i=get_motor_position_counter(1)+get_motor_position_counter(3); // updates distance travelled with get motor position counters
		} 
	}
}

void armmotor(int power)
{
	motor(MOT_ARM, power);
}

//arm library for LINK

void gate(int position)
{
	set_servo_position(3,position);
	msleep(200);
}

void catcher(int position)
{
	set_servo_position(2,position);
	msleep(200);
}




#endif
