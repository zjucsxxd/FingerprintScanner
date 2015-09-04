#include "stdafx.h" // Header which contains the MFC libraries
#include "FingerprintControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FP_BITMAP fp_bmp256(240, 216);

BOOL Save(LPCTSTR szFilePath, FP_BITMAP* fp_bmp, void* p)
{
	CFile f;
	if (!f.Open(szFilePath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite))
		return FALSE;

	int w = fp_bmp->bmInfo.bmiHeader.biWidth;
	int h_org = fp_bmp->bmInfo.bmiHeader.biHeight;
	int h = abs(h_org);

	fp_bmp->bmInfo.bmiHeader.biHeight = h;
	f.Write(fp_bmp, sizeof(FP_BITMAP)); // Transform f in a FP_BITMAP with the correct size
	fp_bmp->bmInfo.bmiHeader.biHeight = h_org;

	for (int i = h - 1; i >= 0; i--) // Write the image in the file line by line
		f.Write((BYTE*)p + i*w, w);

	f.Close();

	return TRUE;
}

void Open()
{
	if (!comm_open_serial(portNumber, 9600)) // Open port
	{
		return;
	}

	if (oem_open() < 0) // Open communication with the device
	{
		return;
	}
}

void Close()
{
	oem_close(); // Close communication with the device
	comm_close(); // Close port
}

int Capturing(BOOL bBest)
{
	while (acquire)
	{
		// Wait until a finger is pressed
		if (oem_capture(bBest) < 0) // Capture the image
		{
			return -1;
		}
		ui_polling();
	}
	return -1;
}

int LoadingImage(void)
{
	if (oem_get_image() < 0) // Download the image from the device
	{
		return -1;
	}
	else if (0)
	{
		// Definition of the file name
		// To change regarding to the identification chosen
		static int gnImageNumber = 1;
		CreateDirectory(_T("C:\\_"), NULL);
		CString strFileName;
		strFileName.Format(_T("C:\\_\\Image%03d.bmp"), gnImageNumber++);

		Save(strFileName, &fp_bmp256, gbyImg8bit); // Save the image
	}
	return 0;
}

void GetImage()
{
	oem_cmos_led(true); // LED ON

	acquire = TRUE;

	if (Capturing() < 0) // Acquire image
		return;

	oem_cmos_led(false); // LED OFF

	if (LoadingImage() < 0) // Download image
		return;

	ui_polling();

	acquire = FALSE;
}

int main(int argc, char* argv[])
{
	portNumber = 4;

	Open();
	GetImage();
	Close();

	return 0;
}
