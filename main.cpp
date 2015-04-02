#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

#include "custom_bitmap.h"
#include "Setup.h"

using namespace std;

const int CENTER = -1;
const int n_colors = 64;
const double e = exp(1);
const double pi = atan(1) * 4;

enum StyleList {LogLog, Log, Sqrt, Line, Quad, Sine, Clean};

double distanceFromCenter(double a, double b);
double getStyle(double iter, double maxIter, int style);

void convert(int x, int y, double &newx, double &newy, Setup set0);
void renderBMP(Setup set0);
void printTestPage();

void abortRender(int &a)
{
	a = 0;
	string input;
	do
	{
		cin >> input;
	} while (input != "abort");
	a = 1;
}

int userInput(Setup &set0);

int main()
{
	Setup set1;

	double distance;
	double mathx, mathy;
	double newx, newy, tempx;
	double xSq, ySq;
	double final_style;

	int width, height;
	int iter_limit;
	int style1;
	int isMandel = 1;

	char draw;
	char colors[n_colors] = "#@&%8HMWN0OVUBDG€3CXYZPLftjil1I()/|[]{}pqdbymwnoeacsxvurz;:+>=-";

	printTestPage();

	while (!userInput(set1))
	{
		height = int(set1.getHeight());
		width = int(set1.getWidth());
		iter_limit = set1.getMaxIter();
		style1 = set1.getStyle1();

		for (int y=0; y<height; y++)
			for (int x=0; x<width; x++)
			{
				convert(x, y, mathx, mathy, set1);

				newx = mathx;
				newy = mathy;

				isMandel = 1;
				draw = ' ';

				for (int iter = 0; iter < iter_limit; iter++)
				{
					xSq = newx*newx;
					ySq = newy*newy;

					if (4 <= xSq + ySq)
					{
						isMandel=0;
						final_style = getStyle(iter, iter_limit, style1);

						if (final_style < 0) final_style = 0;
						else if (final_style > 1) final_style = 1;

						draw = colors [(int)(n_colors * final_style)];

						break;
					}

					tempx = xSq - ySq;
					newy = (newx + newx) * newy;
					newx = tempx;
					newy += mathy;
					newx += mathx;
				}

				distance=distanceFromCenter(mathx,mathy);

				if (distance == CENTER)
					cout << '+';
				else if (isMandel == 1)
					cout << ' ';
				else
					cout << draw;

			}
	}

	return 0;
}

double distanceFromCenter(double a, double b)
{
	if (a==0 && b==0)
		return -1;
	return sqrt(a*a + b*b);
}

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
		iter = 1;
		iter = iter / n_colors;
	}

	// 7-9:
	else // always pick darkest
	{
		iter = n_colors - 1;
		iter = iter / n_colors;
	}

	return iter;
}

int userInput(Setup &set0)
{
	string command;
	cout << "> ";
	cin >> command;

	if		(command == "w") set0.goUp(5);
	else if (command == "s") set0.goDown(5);
	else if (command == "a") set0.goLeft(5);
	else if (command == "d") set0.goRight(5);

	else if (command == "q") set0.zoomIn(-1);
	else if (command == "e") set0.zoomIn(1);
	else if (command == "Q") set0.zoomIn(-0.25);
	else if (command == "E") set0.zoomIn(0.25);

	else if (command == "style")
	{
		string styleNames[7] = { "LogLog", "Log", "Square Root", "Linear", "Quadratic", "Sine", "High Contrast" };

		cout << "Current style: " << styleNames[set0.getStyle1()] << endl;
		cout << "Set new style? (y/n): ";
		cin >> command;

		if (command == "y" || command == "Y" || command == "yes")
		{
			int s1;

			for (int i = 0; i < 7; i++)
				cout << i << "=" << styleNames[i] << ", ";

			cout << endl;
			cout << "Style number:";
			cin >> s1;

			set0.setStyle(s1);
		}
	}

	else if (command == "iter")
	{
		int iter;

		cout << "Current iteration limit: " << set0.getMaxIter() << endl;
		cout << "New iteration limit: ";
		cin >> iter;

		set0.setMaxIter(iter);
	}

	else if (command == "reset" || command=="render")
	{
		set0.reset();
	}

	else if (command == "getinfo")
	{
		double x, y;
		convert(40, 39, x, y, set0);

		cout << "Position: (" << x << ", " << y << "i)" << endl;
		cout << "Zoom level: " << "2^" << set0.getZoom() << " (" << pow(2, set0.getZoom()) << "x)";
		cout << endl;

		return userInput(set0);
	}

	else if (command == "export")
	{

		renderBMP(set0);
		return userInput(set0);
	}

	else if (command == "help")
	{
		cout		 << "\t" << "w"			<< "\t - \t" << "move up" << endl;
		cout << endl << "\t" << "a"			<< "\t - \t" << "move left" << endl;
		cout << endl << "\t" << "s"			<< "\t - \t" << "move down" << endl;
		cout << endl << "\t" << "d"			<< "\t - \t" << "move right" << endl;
		cout << endl << "\t" << "q"			<< "\t - \t" << "normal zoom out" << endl;
		cout << endl << "\t" << "Q"			<< "\t - \t" << "quarter zoom out" << endl;
		cout << endl << "\t" << "e"			<< "\t - \t" << "normal zoom in" << endl;
		cout << endl << "\t" << "E"			<< "\t - \t" << "normal zoom in" << endl;
		cout << endl << "\t" << "style"		<< "\t - \t" << "view and set gradient color picking style" << endl;
		cout << endl << "\t" << "iter"		<< "\t - \t" << "show and change iteration limit" << endl;
		cout << endl << "\t" << "render"	<< "\t - \t" << "redraw image and reset position and zoom" << endl;
		cout << endl << "\t" << "getinfo"	<< "\t - \t" << "get current position info" << endl;
		cout << endl << "\t" << "help"		<< "\t - \t" << "show this" << endl;
		cout << endl << "\t" << "export"	<< "\t - \t" << "exports the current screen in .bmp format" << endl;
		cout << endl << "\t" << "exit" << "\t - \t" << "exit" << endl;

		return userInput(set0);
	}

	else if (command == "exit")
		return 1;

	else
	{
		cout << "Unrecognised command. Try entering 'help'" << endl;
		return userInput(set0);
	}

	return 0;
}

void printTestPage()
{
	cout << endl	<< "Set the console font to \"raster font\" with size 8x8 pixels." << endl << endl;
	cout			<< "Maximize the console window" << endl << endl;
	cout			<< "Type 'render' to close this screen or 'help' for list of commands." << endl << endl;
}

void renderBMP(Setup set0)
{
	double mathx, mathy;
	double newx, newy;
	double xSq, ySq;
	double final_style;
	double scale;

	DWORD color;
	color = 0x59A9FF;
	BYTE padding;

	string filename;
	string path;

	clock_t totalClock;

	char inputChar;
	char * charTemp = new (char);

	int width, height;
	int minDimension;
	int iter_limit;
	int style1;
	int renderNo=0;
	int percent;

	iter_limit = set0.getMaxIter();
	style1 = set0.getStyle1();

	BMP_file bitmap01;

	fstream bmpfile;
	fstream logfile;

	cout << "Image width: ";
	cin >> width;
	cout << "Image height: ";
	cin >> height;

	cout << "Exporting will take approx " << 0.003*sqrt(width*height) << "s";

	minDimension = width;
	if (height < minDimension)
		minDimension = height;

	scale = log(minDimension / 80) / log(2);

	set0.zoomIn(scale);
	set0.setRes(width, height);

	padding = ((4 - ((width * 3) % 4)) % 4);
	Pixel tempPix;

	path = "images\\";
	cout << "File name: ";
	cin >> filename;

	path.append(filename);
	path.append(".bmp");

	bmpfile.open(path.c_str());

	if (bmpfile.is_open())
	{
		cout << "Overwrite? (y/n) ";
		cin >> inputChar;

		if (inputChar == 'n')
		{
			cout << "Export canceled." << endl;
			bmpfile.close();
			return;
		}
	}

	bmpfile.close();
	bmpfile.open(path.c_str(), fstream::out | fstream::trunc | fstream::binary);
	logfile.open("log.txt", fstream::out | fstream::app);

	cout << "Exporting... ";

	totalClock = clock();

	bitmap01.setHeader(width, height);

	for (int i = 0; i < 14; i++)
	{
		*charTemp = bitmap01.get_file_header(i);
		bmpfile.write(charTemp, sizeof(BYTE));
	}
	for (int i = 0; i < 40; i++)
	{
		*charTemp = bitmap01.get_bitmap_header(i);
		bmpfile.write(charTemp, sizeof(BYTE));
	}
	percent = 0;
	for (int y = 0; y<height; y++)
	{
		if (int(100 * (float(y) / height)) > percent)
		{
			percent = int(100 * (float(y) / height));
			cout << percent << "% ";
		}

		for (int x = 0; x < width; x++)
		{
			convert(x, y, mathx, mathy, set0);

			newx = mathx;
			newy = mathy;

			for (int iter = 0; iter < iter_limit; iter++)
			{
				xSq = newx*newx;
				ySq = newy*newy;

				if (4 <= xSq + ySq)
				{
					final_style = getStyle(iter, iter_limit, style1);

					if (final_style > 1)
						final_style = 0;

					break;
				}

				newy = (newx + newx) * newy;
				newx = xSq - ySq;
				newy += mathy;
				newx += mathx;

				if (iter == iter_limit - 1)
					final_style = 0;
			}

			tempPix.setRed(89 * final_style);
			tempPix.setGreen(169 * final_style);
			tempPix.setBlue(255 * final_style);

			*charTemp = tempPix.getBlue();
			bmpfile.write(charTemp, sizeof(BYTE));

			*charTemp = tempPix.getGreen();
			bmpfile.write(charTemp, sizeof(BYTE));

			*charTemp = tempPix.getRed();
			bmpfile.write(charTemp, sizeof(BYTE));

		}
		for (int x = 0; x < bitmap01.get_padding(); x++)
		{
			*charTemp = 0;
			bmpfile.write(charTemp, sizeof(BYTE));
		}
	}

	totalClock = clock() - totalClock;
	convert(40, 39, mathx, mathy, set0);

	logfile << "filename:"  <<  filename << ".bmp" << endl;
	logfile << "width: " << width << endl;
	logfile << "height: " << height << endl;
	logfile << "file size: " << ((width*height * 3) + 54) / (1024 * 1024) << "MiB ("<< width*height*3 + 54 << "B)" << endl;
	logfile << "iterations: " << iter_limit << endl;
	logfile << "x: " << mathx << endl;
	logfile << "y: " << mathy << "i" << endl;
	logfile << "zoom: 2^" << set0.getZoom() << "x" << endl;
	logfile << "clocks to complete: " << totalClock << endl;
	logfile << "seconds: " << float(totalClock) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < 32; i++)
		logfile << "*";
	logfile << endl;

	cout << endl << "Total time: " << float(totalClock) / CLOCKS_PER_SEC << "s" << endl << endl;

	delete charTemp;

	bmpfile.close();
	logfile.close();
	bmpfile.open(path.c_str());

	if (bmpfile.is_open())
		cout << "Exporting done. File saved: " << path << endl;
	else
		cout << "Error saving the file." << endl;

	bmpfile.close();
}
