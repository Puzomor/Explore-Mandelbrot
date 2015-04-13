#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED

#include <cmath>
#include <fstream>

const int name_length = 32 + 1;
const double e = exp(1);
const double pi = atan(1) * 4;

enum StyleList {LogLog, Log, Sqrt, Line, Quad, Sine, Clean};

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

	void loadSet(std::fstream &file);
	void saveSet(std::fstream &file, char name[name_length]);
	void setStyle	(int s1);
	void setMaxIter	(int iter) { maxIter = iter; }
	void setRes		(double width, double height);

	void goUp(double amount){ offsety += multip(amount); }
	void goDown(double amount){ offsety -= multip(amount); }
	void goRight(double amount){ offsetx += multip(amount); }
	void goLeft(double amount){ offsetx -= multip(amount); }
	void zoomIn(double amount){ zoom += amount; }
	void reset();

	Setup();

	Setup(double zoom, double offset_x, double offset_y, double width, double height, int max_iter, int style1);
};

void convert(int x, int y, double &newx, double &newy, Setup set0);
void renderBMP(Setup set0);

double getStyle(double iter, double maxIter, int style);

int userInput(Setup &set0);

#endif // SETUP_H_INCLUDED
