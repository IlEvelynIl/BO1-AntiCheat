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
	bool MapFastFileValid(string map);
	bool ExtraFilesExist();
	string GetPatchMD5(string path);
};