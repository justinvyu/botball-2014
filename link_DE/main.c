// Created on Fri March 28 2014

#include "drive.h"
#include "generic.h"

//define main for main()
#define MAIN

//state system
#define s_INIT 100								// initialize
#define s_BLOCKADE 400							// put down the blockade at the bridge
#define s_TRIBBLE_ONE 300						// get the first tribble pile
#define s_TRIBBLE_TWO 200 						// get the second tribble pile
#define s_SCORE 500								// points
#define s_END 600								// end

#define L_SENSOR 0 								// add light sensor port later

#define COL_GREEN 0 							// camera channels green/pink
#define COL_PINK 1

#define S_GATE 0								// gate servo positions
#define S_CATCHER 2

#define S_OPEN 213								// servo open/closed positions
#define s_CLOSE 1760

#ifdef MAIN
int main()
{
	currstate = s_INIT;
	state(s_INIT)
	{
		next(s_BLOCKADE);
	}
	
	state(s_BLOCKADE)
	{
		next(s_TRIBBLE_ONE);
	}
	
	state(s_TRIBBLE_ONE)
	{
		next(s_TRIBBLE_TWO);
	}
	
	state(s_TRIBBLE_TWO)
	{
		next(s_SCORE);
	}
	
	state(s_SCORE)
	{
		next(s_END);
	}
	
	state(s_END)
	{
		
	}
}
#endif
