/*#define StartPosition  2047
#define EndPosition    310
#define RoboStep       30
#define RoboTimer      400
*/
void slow_servo(int time_int, int s_pos, int step, int endpos)
{
	int position;
	printf ("begin");
	enable_servos();
	
	for (position = StartPosition; position <= EndPosition; position -= RoboStep)
	{
		set_servo_position(2,position);
		msleep (RoboTimer);
		printf ("%d", position);
	}
}
