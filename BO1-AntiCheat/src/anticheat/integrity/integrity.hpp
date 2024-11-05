#include <Windows.h>

#include <TlHelp32.h>

#include <psapi.h>

#include <iostream>

#include <filesystem>

#include <map>

#include <vector>

namespace anticheat {
	namespace integrity {
		void Initialize();
		bool IsFastfileValid(std::string map);
		bool DoExtraFilesExist();
		std::string GetFileMD5(std::string path);
		bool IsStealthPatchInjected();
		bool IsModFileValid();
		std::string GetActiveCheatingBinds();
		bool IsSuspiciousModule(HMODULE hModule, HANDLE hProcess);
		std::string GetModifiedFastfiles();
	} // integrity
} // anticheat