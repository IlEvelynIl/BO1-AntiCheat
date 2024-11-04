#include <Windows.h>

#include <TlHelp32.h>

#include <psapi.h>

#include <iostream>

#include <filesystem>

#include <map>

#include <vector>

namespace anticheat {
	namespace integrity {
		void SetupIntegrityHashes();
		bool IsFastfilePatchValid(std::string map);
		bool DoExtraFilesExist();
		std::string GetFileMD5(std::string path);
		bool IsStealthPatchDLLPresent();
		bool IsModFileValid();
		std::string LookForActiveCheatingBinds();
	} // integrity
} // anticheat