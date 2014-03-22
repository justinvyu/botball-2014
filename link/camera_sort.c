// Created on Fri March 21 2014

#include "camera.h" //camera, drive, generic functions
#include "drive.h"
#include "generic.h"

#define MOT_TREAD 2

#define COL_GREEN 0 //camera channels green/pink
#define COL_PINK 1

void init()
{
	//add more stuff if need to do moar
	camera_open(MED_RES);
}

int get_object_count_both(int ch1, int ch2){
	if(get_object_count(ch1) < 1 && get_object_count(ch2) < 1){
		return 0;
	}
	return 1;
}

int main()
{
	init();
	
	while(1)
	{
		//spin tread until see a tribble
		while(get_object_count_both(COL_GREEN, COL_PINK) == 0){
			mav(MOT_TREAD, 700);
			msleep(20);
		}
		if(cam_area(COL_GREEN) > 50) //gets area of blob if present
		{
			//if green, push into green basket
			printf("green");
		}
		else if(cam_area(COL_PINK) > 50)
		{
			//if pink, push into pink basket
			printf("pink");
		}
		else
		{
			camera_update();
		}
	}
}
