#include <iostream>
#include <limits>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>

#include "screen_manip.h"
#include "custom_bitmap.h"
#include "Setup.h"

using namespace std;

enum Directions {Up = 1, Left = 2, Down = 3, Right = 4};
string styleNames[7] = { "LogLog", "Log", "Square Root", "Linear", "Quadratic", "Sine", "High Contrast" };

const int CENTER = -1;
const int n_colors = 64;

void printTestPage();

int main()
{
	Setup set1;// (5, 0, 0, 80, 78, 500, 1);
	Setup set2 (5, 0, 0, 80, 78, 500, 1);

	fstream saveFile;

	clock_t totalClock;

	double mathx, mathy;
	double newx, newy, tempx;
	double xSq, ySq;
	double final_style;

	int width, height;
	int iter_limit;
	int style1;
	int isMandel = 1;
	int choice = 0;
	int xAlt, yAlt;

	char draw;
	char screenArr[80 * 78] = {};
	char* screenPtr = NULL;
	char colors[n_colors] = "#@&%8HMWN0OVUBDG€3CXYZPLftjil1I()/|[]{}pqdbymwnoeacsxvurz;:+>=-";

	printTestPage();
	choice = userInput (set1);

	while (choice != -1)
	{
		height = int (set1.getHeight());
		width = int (set1.getWidth());
		iter_limit = set1.getMaxIter();
		style1 = set1.getStyle1();
		xAlt = 0;
		yAlt = 0;

		if (choice == Left)
		{
			shiftRight (screenArr);
			width = 5;
		}
		else if (choice == Right)
		{
			shiftLeft (screenArr);
			xAlt = 75;
		}
		else if (choice == Down)
		{
			shiftUp (screenArr);
			height = 5;
		}
		else if (choice == Up)
		{
			shiftDown (screenArr);
			yAlt = 73;
		}

		totalClock = clock();

		for (int y = yAlt; y < height; y++)
		{
			for (int x = xAlt; x < width; x++)
			{
				convert (x, y, mathx, mathy, set1);

				newx = mathx;
				newy = mathy;

				isMandel = 1;

				for (int iter = 0; iter < iter_limit; iter++)
				{
					xSq = newx * newx;
					ySq = newy * newy;

					if (4 <= xSq + ySq)
					{
						isMandel = 0;
						final_style = getStyle (iter, iter_limit, style1);

						if (final_style < 0) final_style = 0;
						else if (final_style > 1) final_style = 1;

						draw = colors [ (int) (n_colors * final_style)];

						break;
					}

					tempx = xSq - ySq;
					newy = (newx + newx) * newy;
					newx = tempx;
					newy += mathy;
					newx += mathx;
				}

				screenPtr = & (screenArr[x + (77 - y) * 80]);

				if (mathx == 0 && mathy == 0)
				{
					*screenPtr = '+';
				}
				else if (isMandel == 1)
				{
					*screenPtr = ' ';
				}
				else
				{
					*screenPtr = draw;
				}
			}
		}

		totalClock = clock() - totalClock;
		cout << "calculation time: " << float (totalClock) / CLOCKS_PER_SEC << "s" << endl;

		printScreen (screenArr);

		choice = userInput (set1);
	}

	return 0;
}


int userInput (Setup& set0)
{
	string command;
	cout << "> ";
	cin >> command;

	if (command == "w")
	{
		set0.goUp (5);
		return Up;
	}
	else if (command == "a")
	{
		set0.goLeft (5);
		return Left;
	}
	else if (command == "s")
	{
		set0.goDown (5);
		return Down;
	}
	else if (command == "d")
	{
		set0.goRight (5);
		return Right;
	}


	else if (command == "q") set0.zoomIn (-1);
	else if (command == "e") set0.zoomIn (1);
	else if (command == "Q") set0.zoomIn (-0.25);
	else if (command == "E") set0.zoomIn (0.25);

	else if (command == "style")
	{
		cout << "Current style: " << styleNames[set0.getStyle1()] << endl;
		cout << "Set new style? (y/n): ";
		cin >> command;

		if (command == "y" || command == "Y" || command == "yes")
		{
			cout << endl;

			int s1;

			for (int i = 0; i < 7; i++)
				cout << i << "=" << styleNames[i] << ", ";

			cout << endl;
			cout << "Style number: ";

			while (! (cin >> s1))
			{
				cin.clear();
				cin.ignore (numeric_limits<streamsize>::max(), '\n');
				cout << "Non-integer value entered!\n";
				cout << "Style number: ";
			}

			set0.setStyle (s1);
		}

		cout << endl;

		return userInput (set0);
	}

	else if (command == "iter")
	{
		int iter;

		cout << "Current iteration limit: " << set0.getMaxIter() << endl;
		cout << endl;
		cout << "New iteration limit: ";

		while (! (cin >> iter))
		{
			cin.clear();
			cin.ignore (numeric_limits<streamsize>::max(), '\n');
			cout << "Non-integer value entered!" << endl;
			cout << "New iteration limit: ";
		}

		cout << endl;

		set0.setMaxIter (iter);

		return userInput (set0);
	}

	else if (command == "reset")
	{
		set0.reset();
	}

	else if (command == "load")
	{
		fstream saveFile;
		saveFile.open ("saves.dat", fstream::in | fstream::binary);
		set0.loadSet (saveFile);
		saveFile.close();

		return userInput (set0);
	}

	else if (command == "save")
	{
		char name[name_length] = {};

		cout << endl;
		cout << "- " << name_length << " characters max, type \'cancel\' to cancel" << endl;
		cout << endl;
		cout << "Name for the save: ";

		cin.ignore();
		cin.get (name, name_length);
		if (strcmp (name, "cancel") == 0)
			return userInput (set0);

		fstream saveFile;

		saveFile.open ("saves.dat", fstream::out | fstream::binary | fstream::app);
		set0.saveSet (saveFile, name);
		saveFile.close();

		cout << endl;

		return userInput (set0);
	}

	else if (command == "info")
	{
		double x, y;
		convert (40, 39, x, y, set0);

		cout << endl;

		cout << "Position: (" << x << ", " << y << "i)" << endl;
		cout << "Zoom level: " << "2^" << set0.getZoom() << " (" << pow (2, set0.getZoom()) << "x)";
		cout << endl;

		cout << endl;

		return userInput (set0);
	}

	else if (command == "export")
	{
		renderBMP (set0);

		cout << endl;

		return userInput (set0);
	}

	else if (command == "help")
	{
		cout << "\n\t" << "w"			<< "\t - \t" << "move up\n";
		cout << "\n\t" << "a"			<< "\t - \t" << "move left\n";
		cout << "\n\t" << "s"			<< "\t - \t" << "move down\n";
		cout << "\n\t" << "d"			<< "\t - \t" << "move right\n";
		cout << "\n\t" << "q"			<< "\t - \t" << "normal zoom out\n";
		cout << "\n\t" << "Q"			<< "\t - \t" << "quarter zoom out\n";
		cout << "\n\t" << "e"			<< "\t - \t" << "normal zoom in\n";
		cout << "\n\t" << "E"			<< "\t - \t" << "quarter zoom in\n";
		cout << "\n\t" << "style"		<< "\t - \t" << "views and sets drawing style\n";
		cout << "\n\t" << "iter"		<< "\t - \t" << "shows and changes iteration limit\n";
		cout << "\n\t" << "draw"		<< "\t - \t" << "redraws image\n";
		cout << "\n\t" << "reset"		<< "\t - \t" << "reset to default position and settings\n";
		cout << "\n\t" << "info"		<< "\t - \t" << "shows mathematical coordinates and zoom\n";
		cout << "\n\t" << "save"		<< "\t - \t" << "saves current coordinates and zoom\n";
		cout << "\n\t" << "reset"		<< "\t - \t" << "loads saved coordinates and zoom\n";
		cout << "\n\t" << "export"		<< "\t - \t" << "exports the current screen in .bmp format\n";
		cout << "\n\t" << "help"		<< "\t - \t" << "shows this\n";
		cout << "\n\t" << "exit" 		<< "\t - \t" << "exits the program\n";
		cout << endl;

		return userInput (set0);
	}

	else if (command == "exit")
		return -1;

	else if (command == "draw")
		return 0;

	else
	{
		cout << "Unrecognized command. Try entering \'help\'" << endl;
		cout << endl;
		return userInput (set0);
	}

	return 0;
}

void renderBMP (Setup set0)
{
	int word_length;
	unsigned int max_word_length = 200 * 1024 * 1024; // 200 MB
	int position;
	int width, height;
	int minDimension;
	int iter_limit;
	int style1;
	int percent;

	double mathx, mathy;
	double newx, newy;
	double xSq, ySq;
	double final_style;
	double scale;

	string filename;
	string path;

	clock_t totalClock;

	char inputChar;
	char* charTemp = new (char);

	BYTE* tempBytePtr = new (BYTE);

	iter_limit = set0.getMaxIter();
	style1 = set0.getStyle1();

	BMP_file bitmap01;

	fstream bmpfile;
	fstream logfile;

	// define image width and height
	cout << endl;
	cout << "Image width: ";
	while (! (cin >> width))
	{
		cin.clear();
		cin.ignore (numeric_limits<streamsize>::max(), '\n');
		cout << "Non-integer value entered!" << endl;
		cout << "Image width: ";
	}

	cout << "Image height: ";
	while (! (cin >> height))
	{
		cin.clear();
		cin.ignore (numeric_limits<streamsize>::max(), '\n');
		cout << "Non-integer value entered!" << endl;
		cout << "Image height: ";
	}

	// define word_length
	for (unsigned int i = width * height; i > 0; i--)
	{
		if (i > max_word_length)
		{
			i = max_word_length;
			continue;
		}

		if ( (width * height) % i == 0)
		{
			word_length = i;
			break;
		}
	}

	cout << endl;
	cout << 3 * word_length << "Bytes allocated." << endl;
	cout << endl;

	char* chars = new (char [word_length * 3]);
	Pixel* pixels = new Pixel[word_length];

	// scale image according to width or height, whatever is smaller
	minDimension = width;
	if (height < minDimension)
		minDimension = height;

	// scale image from 80x80 to width x height, keeping the zoom level
	scale = log (minDimension / 80) / log (2);

	set0.zoomIn (scale);
	set0.setRes (width, height);

	// get image name, set destination to images sub-folder
	path = "";
	cout << "File name: ";
	cin >> filename;
	cout << endl;

	// set final path
	path.append (filename);
	path.append (".bmp");

	// try to open a file
	bmpfile.open (path.c_str());

	// warning for file overwrite
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

	// close the dummy file, open the file for output and get log file ready
	bmpfile.close();
	bmpfile.open (path.c_str(), fstream::out | fstream::trunc | fstream::binary);
	if (!bmpfile.is_open())
	{
		cout << endl;
		cout << "File can't be created, export canceled!" << endl;
		cout << endl;
		return;
	}
	logfile.open ("log.txt", fstream::out | fstream::app);

	cout << "Exporting... ";

	// get start time
	totalClock = clock();

	// generate virtual header
	bitmap01.setHeader (width, height);

	// write file header
	tempBytePtr = bitmap01.get_file_header();
	bmpfile.write (reinterpret_cast<char*> (tempBytePtr), sizeof (BYTE) * 14);

	// write bitmap header
	tempBytePtr = bitmap01.get_bitmap_header();
	bmpfile.write (reinterpret_cast<char*> (tempBytePtr), sizeof (BYTE) * 40);

	percent = 0;
	// go trough every horizontal pixel-line in image
	for (int y = 0; y < height; y++)
	{
		// output progress bar
		if (int (100 * (float (y) / height)) > percent)
		{
			percent = int (100 * (float (y) / height));
			cout << percent << "% ";
		}

		// go trough every pixel in a row
		for (int x = 0; x < width; x++)
		{
			position = x + y * width;
			// convert bitmap pixels to complex plane coordinates
			convert (x, y, mathx, mathy, set0);

			// save current position in temporary values
			newx = mathx;
			newy = mathy;

			// determine if it is within a set
			for (int iter = 0; iter < iter_limit; iter++)
			{
				// define squares
				xSq = newx * newx;
				ySq = newy * newy;

				// try to escape the set
				if (4 <= xSq + ySq)
				{
					final_style = getStyle (iter, iter_limit, style1);

					if (final_style > 1)
						final_style = 0;

					break;
				}

				// multiply complex number, add the original number to it
				newy = (newx + newx) * newy;
				newx = xSq - ySq;
				newy += mathy;
				newx += mathx;

				// if it's the last iteration
				if (iter == iter_limit - 1)
					final_style = 0;
			}

			pixels[position % word_length].setRed (89 * final_style);
			pixels[position % word_length].setGreen (169 * final_style);
			pixels[position % word_length].setBlue (255 * final_style);

			if (position % word_length == word_length - 1)
			{
				for (int i = 0; i < word_length; i++)
				{
					chars[i * 3 + 0] = pixels[i].getBlue();
					chars[i * 3 + 1] = pixels[i].getGreen();
					chars[i * 3 + 2] = pixels[i].getRed();
				}

				bmpfile.write (chars, word_length * 3 * sizeof (BYTE));
			}
		}

		// append padding to every horizontal line
		for (int x = 0; x < bitmap01.get_padding(); x++)
		{
			*charTemp = 0;
			bmpfile.write (charTemp, sizeof (BYTE));
		}
	}

	// get time elapsed
	totalClock = clock() - totalClock;

	// get screen center coordinates
	convert (40, 39, mathx, mathy, set0);

	// log various information about the file and the perspective in the set
	logfile << "filename: "  <<  filename << ".bmp" << endl;
	logfile << "width: " << width << endl;
	logfile << "height: " << height << endl;
	logfile << "file size: " << ( (width * height * 3) + 54) / (1024 * 1024) << "MiB (" << width* height * 3 + 54 << "B)" << endl;
	logfile << "iterations: " << iter_limit << endl;
	logfile << "x: " << mathx << endl;
	logfile << "y: " << mathy << "i" << endl;
	logfile << "zoom: 2^" << set0.getZoom() << "x" << endl;
	logfile << "clocks to complete: " << totalClock << endl;
	logfile << "seconds: " << float (totalClock) / CLOCKS_PER_SEC << "s" << endl;
	for (int i = 0; i < 32; i++)
		logfile << "*";
	logfile << endl;

	cout << endl << "Total time: " << float (totalClock) / CLOCKS_PER_SEC << "s" << endl << endl;

	// get rid of dynamic allocated objects
	delete charTemp;
	delete chars;
	delete pixels;

	// close the files
	bmpfile.close();
	logfile.close();
	bmpfile.open (path.c_str());

	// test for existence of the file
	if (bmpfile.is_open())
		cout << "Exporting done. File saved: " << path << endl;
	else
		cout << "Error saving the file." << endl;

	// finish
	bmpfile.close();
}

