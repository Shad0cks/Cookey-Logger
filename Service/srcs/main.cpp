#include "Service.h"

tinky tinky_g;

void init_struct()
{
    memset(tinky_g.servicePath, 0, MAX_PATH);
    
    GetModuleFileName(0, tinky_g.servicePath, MAX_PATH);
    
}

int main(int argc, char** argv)
{
    init_struct();

    if (argc == 2)
    {
        if (OpenServiceManager())
            return 1;

        if (strcmp(argv[1], "install") == 0)
            return CreateTKService();
        else if (strcmp(argv[1], "start") == 0)
            return StartTKService();
        else if (strcmp(argv[1], "stop") == 0)
            return StopTKService();
        else if (strcmp(argv[1], "delete") == 0)
            return DeleteTKService();
    }
    else
    {
        SERVICE_TABLE_ENTRY ServiceStartTable[2];
        ServiceStartTable[0].lpServiceName = PROGAM_NAME;
        ServiceStartTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
        ServiceStartTable[1].lpServiceName = NULL;
        ServiceStartTable[1].lpServiceProc = NULL;

        if (StartServiceCtrlDispatcher(ServiceStartTable))
            return 0;
        else if (GetLastError() == ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
            std::cout << "Program not execute as a service\n";
        else
            std::cout << "Service Dispatcher failed\n";
    }
    return (1);
}
