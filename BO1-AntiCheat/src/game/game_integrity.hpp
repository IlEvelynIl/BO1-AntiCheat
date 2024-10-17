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
	bool CommonZombiePatchValid();
	bool Game_ModFrontendPatchValid();
	bool MapFastFileValid(string map);
	bool ExtraFilesExist();
	string GetFileMD5(string path);
};