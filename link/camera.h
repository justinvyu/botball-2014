//general use of the camera
//define TEST if you cant to self test the camera
//#define TEST 0
float _start_time;

void start()
{
	_start_time = seconds();
}
float curr_time()
{
    return seconds()-_start_time;
}
void now()
{
    printf("now %f\n",curr_time());
}

void cam_block(){//updates the camera, and waits until success
	while(!camera_update()) msleep(1);
}

//self tester
//dont use as of 3142014
#ifdef TEST
int main()
{
	int x = 0;
	camera_open(LOW_RES);
	cam_block();
	cam_block();
	start();
	while(1)
	{	
		cam_block();
		x++;
		printf("\n%f",x/curr_time());
	}
}
#endif
