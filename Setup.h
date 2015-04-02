#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

#include <cmath>

class Setup {
	double zoom;
	double offsetx;
	double offsety;
	double width;
	double height;
	int maxIter;
	int style1;

	double multip(double amount) { return amount * pow(2, -zoom); }

public:
	int getMaxIter(){ return maxIter; }
	int getStyle1() { return style1; }
	double getZoom() { return zoom; }
	double getHeight() { return height; }
	double getWidth() { return width; }
	double getOffsetX() { return offsetx; }
	double getOffsetY() { return offsety; }

	void setStyle	(int s1);
	void setMaxIter	(int iter) { maxIter = iter; }
	void setRes		(double width, double height);

	void goUp(double amount){ offsety -= multip(amount); }
	void goDown(double amount){ offsety += multip(amount); }
	void goRight(double amount){ offsetx += multip(amount); }
	void goLeft(double amount){ offsetx -= multip(amount); }
	void zoomIn(double amount){ zoom += amount; }
	void reset();

	Setup();

	Setup(double zoom, double offset_x, double offset_y, double width, double height, int max_iter, int style1);
};

#endif // SETUP_H_INCLUDED
