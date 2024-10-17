#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>
#include "../Constants.h"
#include <iostream>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

class FastFiles
{
public:
	void init();
	bool CommonZombiePatchValid();
	bool Game_ModFrontendPatchValid(); // 06e3b0de38ef784d9bf3fd087187c502
	bool MapFastFileValid(string map);
	bool ExtraFilesExist();
	string GetPatchMD5(string path);
};