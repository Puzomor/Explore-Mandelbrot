#include "screen_manip.h"
#include <iostream>

using namespace std;

void printScreen(char screen[80*78])
{
	cout.write(screen, 80*78);
}

void shiftRight(char screen[80*78])
{
	for (int j = 0; j < 78; j++)
	{
		for (int i = 80-1; i >= 5; i--)
		{
			screen[i + j*80] = screen[(i-5) + j*80];
		}
	}
}

void shiftLeft(char screen[80*78])
{
	for (int j=0; j<78; j++)
	{
		for (int i = 0; i < 80-5; i++)
		{
			screen[i + j*80] = screen[(i+5) + j*80];
		}
	}
}

void shiftUp (char screen[80*78])
{
	for (int j=0; j<78-5; j++)
	{
		for (int i = 0; i < 80; i++)
		{
			screen[i + j*80] = screen[i + (j+5)*80];
		}
	}
}

void shiftDown (char screen[80*78])
{
	for (int j = 78-1; j >= 5; j--)
	{
		for (int i = 0; i < 80; i++)
		{
			screen[i + j*80] = screen[i + (j-5)*80];
		}
	}
}

