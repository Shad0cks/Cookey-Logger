#include "Service.h"

int OpenServiceTinky()
{
    tinky_g.ServiceHandle = OpenService(tinky_g.ServiceManager, SERVICE_NAME, GENERIC_ALL);

    if (tinky_g.ServiceHandle == NULL)
    {
        printError("OpenService", GetLastError());
        return 1;
    }
    return 0;
}


int OpenServiceManager()
{
    tinky_g.ServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (tinky_g.ServiceManager == NULL)
    {
        printError("OpenSCManager", GetLastError());
        return 1;
    }
    return 0;
}

int setServiceStatusTinky(DWORD currentState, DWORD exitCode)
{
    tinky_g.serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    if (currentState != SERVICE_START_PENDING)
        tinky_g.serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    else
        tinky_g.serviceStatus.dwControlsAccepted = 0;
    tinky_g.serviceStatus.dwCurrentState = currentState;
    tinky_g.serviceStatus.dwWin32ExitCode = exitCode;
    tinky_g.serviceStatus.dwServiceSpecificExitCode = 0;
    tinky_g.serviceStatus.dwCheckPoint = 0;
    tinky_g.serviceStatus.dwWaitHint = 0;

    return SetServiceStatus(tinky_g.handleServiceStatus, &tinky_g.serviceStatus) == 0;
}

DWORD nameToPid(const wchar_t *name)
{
    HANDLE snapProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    
    if (snapProcess == INVALID_HANDLE_VALUE)
    {
        printError("CreateToolhelp32Snapshot", GetLastError());
        return 0;
    }

    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(PROCESSENTRY32);

    Process32First(snapProcess, &pEntry);

    do
    {
        if (wcscmp(name, pEntry.szExeFile) == 0)
        {
            CloseHandle(snapProcess);
            return pEntry.th32ProcessID;
        }

    } while (Process32Next(snapProcess, &pEntry));

    CloseHandle(snapProcess);
    return (0);
}

void killProcessByName(const wchar_t* name)
{
    DWORD ProcPID = nameToPid(name);
    HANDLE ProcHANDLE = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcPID);

    TerminateProcess(ProcHANDLE, 0);
    CloseHandle(ProcHANDLE);
}

void printError(std::string func, DWORD code)
{
    std::cout << func << " failed (" << code << ").\n";
}