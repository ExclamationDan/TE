#pragma once




namespace TE
{

	class CTGA
	{
	public:
		unsigned char imageTypeCode;
		short int imageWidth;
		short int imageHeight;
		unsigned char bitCount;
		unsigned char *imageData;


		bool Load(char *filename);



		CTGA(void);
		~CTGA(void);
	};

}