/*#define StartPosition  2047
#define EndPosition    310
#define RoboStep       30
#define RoboTimer      400
*/
void slow_servo(int time_int, int step, int s_pos, int endpos)
{
	int position;
	printf ("begin");
	
	for (position = s_pos; position <= end_pos; position -= step)
	{
		set_servo_position(2,position);
		msleep (time_int);
		printf ("%d", position);
	}
}
