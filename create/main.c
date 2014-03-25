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
#define S_PORT 2
#define	SLEEP 750
#define SP_RIGHT 250
#define SP_LEFT -250

int main()
{
	set_servo_position(S_PORT,OPEN);
	enable_servos();
	create_connect();
	wait_for_start(0);
	shut_down_in(119);
	arm_up();
	
	create_forward(500,750);
	
	create_drive_direct_right(SP_RIGHT,SP_LEFT);
	msleep(750);
	create_stop();
	
	create_forward(500,1250);
	
	create_drive_direct_left(SP_RIGHT,SP_LEFT);
	msleep(750);
	create_stop();
	
	create_forward(100,SP_RIGHT);
	set_servo_position(S_PORT,CLOSED);    
}
