#include "TGA.h"
#include <iostream>

using namespace TE;

CTGA::CTGA(void)
{
}


CTGA::~CTGA(void)
{
}


bool CTGA::Load(char* filename)
{

	FILE *filePtr;
	unsigned char ucharBad;
	short int sintBad;
	long imageSize;
	int colorMode;
	unsigned char colorSwap;

	// Open the TGA file.
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
	{
		return false;
	}

	// Read the two first bytes we don't need.
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

	// Which type of image gets stored in imageTypeCode.
	fread(&imageTypeCode, sizeof(unsigned char), 1, filePtr);

	// For our purposes, the type code should be 2 (uncompressed RGB image)
	// or 3 (uncompressed black-and-white images).
	if (imageTypeCode != 2 && imageTypeCode != 3)
	{
		fclose(filePtr);
		return false;
	}

	// Read 13 bytes of data we don't need.
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);
	fread(&sintBad, sizeof(short int), 1, filePtr);

	// Read the image's width and height.
	fread(&imageWidth, sizeof(short int), 1, filePtr);
	fread(&imageHeight, sizeof(short int), 1, filePtr);

	// Read the bit depth.
	fread(&bitCount, sizeof(unsigned char), 1, filePtr);

	// Read one byte of data we don't need.
	fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

	// Color mode -> 3 = BGR, 4 = BGRA.
	colorMode = bitCount / 8;
	imageSize = imageWidth * imageHeight * colorMode;

	// Allocate memory for the image data.
	imageData = (unsigned char*) ::operator new(sizeof(unsigned char)*imageSize);
	//imageData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);

	// Read the image data.
	fread(imageData, sizeof(unsigned char), imageSize, filePtr);

	// Change from BGR to RGB so OpenGL can read the image data.
	for (int imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode)
	{
		colorSwap = imageData[imageIdx];
		imageData[imageIdx] = imageData[imageIdx + 2];
		imageData[imageIdx + 2] = colorSwap;
	}

	fclose(filePtr);
	return true;



}
