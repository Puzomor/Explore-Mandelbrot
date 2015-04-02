#include "Setup.h"

Setup::Setup()
{
	reset();
}

Setup::Setup(double z, double x, double y, double w, double h, int miter, int s1)
{
	zoom = z;
	offsetx = x;
	offsety = y;
	width = w;
	height = h;
	maxIter = miter;
	style1 = s1;
}

void Setup::setStyle(int s1)
{
	style1 = s1;
}

void Setup::reset()
{
	zoom = 5;
	offsetx = 0;
	offsety = 0;
	width = 80;
	height = 78;
	maxIter = 50;
	style1 = 3;
}

void Setup::setRes(double width, double height)
{
	this->width = width;
	this->height = height;
}
