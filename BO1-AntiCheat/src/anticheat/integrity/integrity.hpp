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
		std::string GetExtraFilesInZone();
		bool IsStealthPatchInjected();
		bool IsModFileValid();
		std::string GetModifiedPlayerStates();
		bool IsSuspiciousModule(HMODULE hModule, HANDLE hProcess);
		std::string GetModifiedFastfiles();
		std::string GetModifiedLangFiles();
	} // integrity
} // anticheat