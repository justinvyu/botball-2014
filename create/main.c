#include "create_functions.h"
#include "createDrive.h"
#include "generic.h"

#define OPEN 1500
#define CLOSED 1200

int main()
{
  //initializing program
  
	set_servo_position(2,OPEN);
	enable_servos();
	create_connect();
	wait_for_start(0);
	shut_down_in(119);
	arm_up();
	
	//drives create to end of start box
	
	create_forward(500,750);
	
	//create turns 90 degrees
	
	create_drive_direct_left(250,-250);
	msleep(750);
	create_stop();
	
	//create drives to blocks
	
	create_forward(500,1250);
	
	//create turns 90 degrees
	
	create_drive_direct_left(250,-250);
	msleep(750);
	create_stop();
	
	create_forward(100,250);    //create drives blocks to pick up position
	set_servo_position(2,CLOSED);     //create picks up blocks
}
