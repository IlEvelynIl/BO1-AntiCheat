#include <Windows.h>

#include <TlHelp32.h>

#include <psapi.h>

#include <iostream>

#include <filesystem>

#include <map>

#include "../Constants.h"

#include <vector>

#include <string>

class GameIntegrity
{
public:
	void init();
	bool IsFastfilePatchValid(string map);
	bool DoExtraFilesExist();
	string GetFileMD5(string path);
	bool IsStealthPatchDLLPresent();
	bool IsModFileValid();
	string LookForActiveCheatingBinds();
};