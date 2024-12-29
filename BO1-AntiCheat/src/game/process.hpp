#include <windows.h>
#include <string>

using namespace std;

namespace game {
	namespace process {
		HANDLE GetBlackOpsProcess();
		bool IsGameOpen();
		std::string GetPathToExe();
		DWORD GetProcessIdByName(const std::wstring& procName);
	} // process
} // game