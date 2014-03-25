/*------------------------------------------------
create main function (blocks) by Tommy Yang
--------------------------------------------------
Function initializes program, then drives create to
end of the start box. It then turns 90 degrees right
and drives to the blocks. It then turns another 90
degrees left and drives to pick up the blocks.
-------------------------------------------------*/

#include "create_functions.h"
#include "createDrive.h"
#include "generic.h"

#define OPEN 1500
#define CLOSED 1200

int main()
{
	set_servo_position(2,OPEN);
	enable_servos();
	create_connect();
	wait_for_start(0);
	shut_down_in(119);
	arm_up();
	
	create_forward(500,750);
	
	create_drive_direct_right(250,-250);
	msleep(750);
	create_stop();
	
	create_forward(500,1250);
	
	create_drive_direct_left(250,-250);
	msleep(750);
	create_stop();
	
	create_forward(100,250);
	set_servo_position(2,CLOSED);    
}
