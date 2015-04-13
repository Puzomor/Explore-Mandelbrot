#include "Setup.h"
#include "custom_bitmap.h"

#include <iostream>
#include <limits>

Setup::Setup() : zoom(5), offsetx(-0.5), offsety(0), width(80), height(78), maxIter(500), style1(1)
{

}

Setup::Setup(double z, double x, double y, double w, double h, int miter, int s1)
: zoom(z), offsetx(x), offsety(y), width(w), height(h), maxIter(miter), style1(s1)
{

}

void Setup::setStyle(int s1)
{
	style1 = s1;
}

void Setup::reset()
{
	zoom = 5;
	offsetx = -0.5;
	offsety = 0;
	width = 80;
	height = 78;
	maxIter = 500;
	style1 = 1;
}

void Setup::setRes(double width, double height)
{
	this->width = width;
	this->height = height;
}

void Setup::saveSet(std::fstream &file, char name[name_length])
{
	file.write(name, sizeof(char) * name_length);
	file.write(reinterpret_cast<char*>(&offsetx), sizeof(offsetx));
	file.write(reinterpret_cast<char*>(&offsety), sizeof(offsety));
	file.write(reinterpret_cast<char*>(&zoom), sizeof(zoom));
}

void Setup::loadSet(std::fstream &file)
{
	char dump[name_length];
	int i = 0;
	int size = sizeof(char) * name_length + sizeof(offsetx) + sizeof(offsety) + sizeof(zoom);
	std::cout << std::endl;
	while (file.read(dump, sizeof(char) * name_length))
	{
		std::cout << "[" << i++ << "] " << dump << std::endl;
		file.read(reinterpret_cast<char*>(&offsetx), sizeof(offsetx));
		file.read(reinterpret_cast<char*>(&offsety), sizeof(offsety));
		file.read(reinterpret_cast<char*>(&zoom), sizeof(zoom));
	}
	std::cout << std::endl << "Enter save number: ";

	while (! (std::cin >> i))
	{
		std::cin.clear();
		std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Non-integer value entered!" << std::endl;
		std::cout << "Enter save number: ";
	}

	std::cout << std::endl;

	if (i==0)
		return;

	file.clear();
	file.seekg(i * size);

	file.read(dump, sizeof(char) * name_length);
	file.read(reinterpret_cast<char*>(&offsetx), sizeof(offsetx));
	file.read(reinterpret_cast<char*>(&offsety), sizeof(offsety));
	file.read(reinterpret_cast<char*>(&zoom), sizeof(zoom));

}

/* ************************************************************************************ */
void convert(int x, int y, double &newx, double &newy, Setup set0)
{
	double z = pow(2,set0.getZoom());
	double ox = set0.getOffsetX();
	double oy = set0.getOffsetY();
	double w = set0.getWidth();
	double h = set0.getHeight();

	newx = x + ox*z;
	newx /= z;
	newx -= (w / 2) / z;

	newy = y + oy*z;
	newy /= z;
	newy -= (h / 2) / z;
}

double getStyle(double iter, double maxIter, int style)
{
	// 0:
	if (style == LogLog) // loglog ftw
	{
		iter = iter + 1;
		iter = log(iter);
		iter = log(iter);
		iter = iter / log(maxIter + 1);
	}

	// 1:
	else if (style == Log) // logarithmic growth: log(iter+1) / log(1000+1)
	{
		iter = iter + 1;
		iter = log(iter);
		iter = iter / log(maxIter + 1);
	}

	// 2:
	else if (style == Sqrt) // exponential growth (square root): sqrt(iter/1000)
	{
		iter = iter / maxIter;
		iter = sqrt(iter);
	}

	// 3:
	else if (style == Line) // linear growth
	{
		iter = iter / maxIter;
	}

	// 4:
	else if (style == Quad) // quadratic growth
	{
		iter = iter / maxIter;
		iter = iter * iter;
	}

	// 5:
	else if (style == Sine) // sinusoidal growth
	{
		iter = iter / maxIter;
		iter = iter + 1.5;
		iter = iter * pi;
		iter = sin(iter);
		iter = iter / 2;
		iter = iter + 0.5;
	}

	// 6:
	else if (style == Clean) // always pick brightest
	{
		iter = maxIter;
	}

	// 7-9:
	else // always pick darkest
	{
		iter = 0;
	}

	return iter;
}

void printTestPage()
{
	std::cout	<< "\nSet the console font to \"raster font\" with size 8x8 pixels.\n\n";
	std::cout	<< "Resize the console window to 80x80\n\n";
	std::cout	<< "Type 'help' for the list of commands.\n\n";
}
