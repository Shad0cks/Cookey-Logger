#ifndef SERVICE_H
#define SERVICE_H

#pragma warning(disable : 4820 5039 4668)

#include <Windows.h>
#include <iostream>
#include <Winsvc.h>
#include <Tlhelp32.h>
#include <userenv.h>
#pragma comment(lib, "userenv.lib")
#include <direct.h>

#define SERVICE_NAME L"tinky"
#define PROGAM_NAME L"winkey.exe"
#define SYSTEM_TOKEN L"winlogon.exe"

typedef struct tinky_s
{
    SC_HANDLE               ServiceHandle;
    SC_HANDLE               ServiceManager;
    SERVICE_STATUS          serviceStatus;
    SERVICE_STATUS_HANDLE   handleServiceStatus;
    HANDLE                  ExitServiceHandle;
    wchar_t                 servicePath[MAX_PATH];
    wchar_t*                pathToLocalAppData;
    STARTUPINFO             startInfo;
    PROCESS_INFORMATION     processInfo;
    HANDLE                  newWinlongToken;
} tinky;

extern tinky tinky_g;

int                 CreateTKService();
int                 StartTKService();
int                 StopTKService();
int                 DeleteTKService();
int                 OpenServiceTinky();
int                 OpenServiceManager();
VOID WINAPI         ServiceMain(DWORD dwNumServicesArgs, LPWSTR * lpServiceArgVectors);
int                 setServiceStatusTinky(DWORD currentState, DWORD exitCode);
DWORD               nameToPid(const wchar_t* name);
void                killProcessByName(const wchar_t* name);
void                printError(std::string func, DWORD code);

#endif