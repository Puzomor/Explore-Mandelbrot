#ifndef CUSTOM_BITMAP_H_INCLUDED
#define CUSTOM_BITMAP_H_INCLUDED

typedef unsigned char BYTE; // 1 Byte = 8 bits
typedef unsigned short WORD; // 2 Bytes = 16 bits
typedef unsigned long DWORD; // 4 Bytes = 32 bits

class Pixel {
	BYTE red;
	BYTE green;
	BYTE blue;
public:
	void setColor(BYTE red, BYTE green, BYTE blue);
	void setColor(DWORD value);
	void setRed(BYTE red) { this->red = red; }
	void setGreen(BYTE green) { this->green = green; }
	void setBlue(BYTE blue) { this->blue = blue; }

	BYTE getRed(){ return red; }
	BYTE getGreen(){ return green; }
	BYTE getBlue(){ return blue; }
};

class Word {
	WORD dec_value;
	WORD word_value;
	BYTE byte[2];
public:
	void setValue(WORD value);

	BYTE getByte(int index) { return byte[index]; }
	WORD getDec() { return dec_value; }
	WORD getWord() { return word_value; }
};

class DWord {
private:
	DWORD dec_value;
	DWORD dword_value;
	BYTE byte[4];
public:
	DWord() : dec_value(0), dword_value(0) {};
	DWord(DWORD value);

	void setValue(DWORD value);

	BYTE getByte(int index) { return byte[index]; }
	DWORD getDec() { return dec_value; }
	DWORD getDword() { return dword_value; }
};

class BMP_file{
	BYTE padding;
	BYTE file_header[14];
	BYTE bitmap_header[40];
public:
	void setHeader(int width, int height);

	BYTE get_file_header(int index);
	BYTE get_bitmap_header(int index);
	BYTE get_padding() { return padding; }
	BYTE* get_file_header() { return file_header; }
	BYTE* get_bitmap_header() { return bitmap_header; }
};

#endif // CUSTOM_BITMAP_H_INCLUDED
