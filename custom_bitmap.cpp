#include "custom_bitmap.h"

void Word::setValue(WORD value)
{
	dec_value = value;

	byte[1] = dec_value >> 8;
	byte[0] = dec_value - (byte[1] << 8);
	word_value = byte[0];
	word_value <<= 8;
	word_value += byte[1];
}

void DWord::setValue(DWORD value)
{
	dec_value = value;

	byte[3] = (dec_value & 0xff000000) >> 8 * 3;
	byte[2] = (dec_value & 0x00ff0000) >> 8 * 2;
	byte[1] = (dec_value & 0x0000ff00) >> 8 * 1;
	byte[0] = (dec_value & 0x000000ff) >> 8 * 0;

	dword_value = byte[0];
	dword_value <<= 8;
	dword_value += byte[1];
	dword_value <<= 8;
	dword_value += byte[2];
	dword_value <<= 8;
	dword_value += byte[3];
}

void BMP_file::setHeader(int width, int height)
{
	DWord fileSize;
	DWord bitmapSize;
	DWord bitmapOffset;
	DWord bitmapHeaderSize;
	DWord imgWidth;
	DWord imgHeight;
	DWord compression;
	DWord horizontalRes;
	DWord verticalRes;
	DWord colorsUsed;
	DWord colorsImportant;

	Word tmpWord;
	Word reserved;
	Word planesNo;
	Word bitsPerPixel;

	padding = (4 - ((width*3) % 4)) % 4;

	int fileSizeInt = width * 3;
	fileSizeInt += padding;
	fileSizeInt *= height;

	tmpWord.setValue(0x4D42);
	bitmapOffset.setValue(54);
	bitmapSize.setValue(fileSizeInt);
	fileSize.setValue(fileSizeInt + bitmapOffset.getDec());
	reserved.setValue(0);
	bitmapHeaderSize.setValue(40);
	imgWidth.setValue(width);
	imgHeight.setValue(height);
	compression.setValue(0);
	horizontalRes.setValue(0);
	verticalRes.setValue(0);
	colorsUsed.setValue(0);
	colorsImportant.setValue(0);
	planesNo.setValue(1);
	bitsPerPixel.setValue(24);

	file_header[0] = tmpWord.getByte(0);
	file_header[1] = tmpWord.getByte(1);

	file_header[2] = fileSize.getByte(0);
	file_header[3] = fileSize.getByte(1);
	file_header[4] = fileSize.getByte(2);
	file_header[5] = fileSize.getByte(3);

	file_header[6] = reserved.getByte(0);
	file_header[7] = reserved.getByte(1);

	file_header[8] = reserved.getByte(0);
	file_header[9] = reserved.getByte(1);

	file_header[10] = bitmapOffset.getByte(0);
	file_header[11] = bitmapOffset.getByte(1);
	file_header[12] = bitmapOffset.getByte(2);
	file_header[13] = bitmapOffset.getByte(3);

	bitmap_header[0] = bitmapHeaderSize.getByte(0);
	bitmap_header[1] = bitmapHeaderSize.getByte(1);
	bitmap_header[2] = bitmapHeaderSize.getByte(2);
	bitmap_header[3] = bitmapHeaderSize.getByte(3);

	bitmap_header[4] = imgWidth.getByte(0);
	bitmap_header[5] = imgWidth.getByte(1);
	bitmap_header[6] = imgWidth.getByte(2);
	bitmap_header[7] = imgWidth.getByte(3);

	bitmap_header[8] = imgHeight.getByte(0);
	bitmap_header[9] = imgHeight.getByte(1);
	bitmap_header[10] = imgHeight.getByte(2);
	bitmap_header[11] = imgHeight.getByte(3);

	bitmap_header[12] = planesNo.getByte(0);
	bitmap_header[13] = planesNo.getByte(1);

	bitmap_header[14] = bitsPerPixel.getByte(0);
	bitmap_header[15] = bitsPerPixel.getByte(1);

	bitmap_header[16] = compression.getByte(0);
	bitmap_header[17] = compression.getByte(1);
	bitmap_header[18] = compression.getByte(2);
	bitmap_header[19] = compression.getByte(3);

	bitmap_header[20] = bitmapSize.getByte(0);
	bitmap_header[21] = bitmapSize.getByte(1);
	bitmap_header[22] = bitmapSize.getByte(2);
	bitmap_header[23] = bitmapSize.getByte(3);

	bitmap_header[24] = horizontalRes.getByte(0);
	bitmap_header[25] = horizontalRes.getByte(1);
	bitmap_header[26] = horizontalRes.getByte(2);
	bitmap_header[27] = horizontalRes.getByte(3);

	bitmap_header[28] = verticalRes.getByte(0);
	bitmap_header[29] = verticalRes.getByte(1);
	bitmap_header[30] = verticalRes.getByte(2);
	bitmap_header[31] = verticalRes.getByte(3);

	bitmap_header[32] = colorsUsed.getByte(0);
	bitmap_header[33] = colorsUsed.getByte(1);
	bitmap_header[34] = colorsUsed.getByte(2);
	bitmap_header[35] = colorsUsed.getByte(3);

	bitmap_header[36] = colorsImportant.getByte(0);
	bitmap_header[37] = colorsImportant.getByte(1);
	bitmap_header[38] = colorsImportant.getByte(2);
	bitmap_header[39] = colorsImportant.getByte(3);
}

void Pixel::setColor(BYTE red, BYTE green, BYTE blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Pixel::setColor(DWORD value)
{
	red		= (value & 0x00ff0000) >> 8 * 2;
	green	= (value & 0x0000ff00) >> 8 * 1;
	blue	= (value & 0x000000ff) >> 8 * 0;
}

BYTE BMP_file::get_file_header(int index)
{
	return file_header[index];
}

BYTE BMP_file::get_bitmap_header(int index)
{
	return bitmap_header[index];
}
