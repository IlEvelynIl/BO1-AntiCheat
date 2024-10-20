#include <Windows.h>

#include <TlHelp32.h>

#include <string>

#include <psapi.h>

#include <iostream>

#include <filesystem>

#include <map>

#include "../Constants.h"

class GameIntegrity
{
public:
	void init();
	bool IsFastfilePatchValid(string map);
	bool DoExtraFilesExist();
	string GetFileMD5(string path);
	bool IsStealthPatchDLLPresent();
};