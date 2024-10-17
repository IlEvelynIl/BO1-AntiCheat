#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>
#include "../Constants.h"
#include <iostream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

class GameIntegrity
{
public:
	void init();
	bool IsCommonZombiePatchValid();
	bool IsGameModFrontendPatchValid();
	bool IsMapFastFileValid(string map);
	bool DoExtraFilesExist();
	string GetFileMD5(string path);
	bool IsStealthPatchDLLPresent();
};