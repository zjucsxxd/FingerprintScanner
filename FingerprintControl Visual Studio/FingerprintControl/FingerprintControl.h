#ifndef FINGERPRINTCONTROL
#define FINGERPRINTCONTROL

#include "OEM.h" // Functions for communication with the device. Provided by ADH-Tech

// Definition of the image type
struct FP_BITMAP
{
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFO bmInfo;
	RGBQUAD bmiColors[255];

	FP_BITMAP(int cx, int cy)
	{
		bmfHdr.bfType = ((WORD)('M' << 8) | 'B');  // "BM"
		bmfHdr.bfSize = sizeof(FP_BITMAP) + cx*cy;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = sizeof(FP_BITMAP);

		bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmInfo.bmiHeader.biWidth = cx;
		bmInfo.bmiHeader.biHeight = -cy;
		bmInfo.bmiHeader.biPlanes = 1;
		bmInfo.bmiHeader.biBitCount = 8;
		bmInfo.bmiHeader.biCompression = 0;
		bmInfo.bmiHeader.biSizeImage = cx*cy;
		bmInfo.bmiHeader.biXPelsPerMeter = 0;
		bmInfo.bmiHeader.biYPelsPerMeter = 0;
		bmInfo.bmiHeader.biClrUsed = 0;
		bmInfo.bmiHeader.biClrImportant = 0;

		RGBQUAD *pals = bmInfo.bmiColors;
		for (int i = 0; i < 256; i++) {
			pals[i].rgbBlue = i;
			pals[i].rgbGreen = i;
			pals[i].rgbRed = i;
			pals[i].rgbReserved = 0;
		}
	}
};

BOOL acquire; // Used to launch the capture

int  Capturing(BOOL bBest = FALSE); // Capture the image on the sensor
int  LoadingImage(void); // Donwload and save the image

int portNumber; // Number of the port for connection

void Open(); // Open connection
void Close(); // Close connection
void GetImage(); // Acquire and save fingerprint image

#endif