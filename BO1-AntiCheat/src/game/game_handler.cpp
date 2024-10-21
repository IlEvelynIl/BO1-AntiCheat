#include <Windows.h>

#include <TlHelp32.h>

#include <string>

#include <psapi.h>

#include <map>

#include "game_handler.hpp"

#include "game_integrity.hpp"

#include "../utilities/memory.hpp"

#include "../constants.h"

#include <vector>

using namespace std;

int GameHandler::GetMapId()
{
    Memory mem;
    return mem.ReadInt(GetBlackOpsProcess(), Constants::C_MAPADDRESS);
}

string GameHandler::GetModName()
{
    Memory mem;
    return mem.ReadString(GetBlackOpsProcess(), Constants::C_MODADDRESS);
}

bool GameHandler::IsModLoaded()
{
    return GetModName() != "";
}

bool GameHandler::IsGameModLoaded()
{
    Memory mem;
    GameIntegrity gi;
    HANDLE handle = GetBlackOpsProcess();

    if (handle == NULL)
    {
        return false;
    }

    vector<HMODULE> hMods(1024);
    DWORD cbNeeded;

    if (EnumProcessModulesEx(handle, hMods.data(), hMods.size() * sizeof(HMODULE), &cbNeeded, LIST_MODULES_ALL)) {
        size_t moduleCount = cbNeeded / sizeof(HMODULE);
        for (size_t i = 0; i < moduleCount; i++) {
            TCHAR szModName[MAX_PATH];
            MODULEINFO modInfo;

            if (GetModuleFileNameEx(handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                if (GetModuleInformation(handle, hMods[i], &modInfo, sizeof(modInfo))) {
                    char modulePath[MAX_PATH];
                    size_t convertedChars = 0;
                    wcstombs_s(&convertedChars, modulePath, szModName, sizeof(modulePath));
                    modulePath[sizeof(modulePath) - 1] = '\0';
                    string dllPath = string(modulePath);

                    if (gi.GetFileMD5(dllPath) == Constants::GAME_MOD_DLL)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool GameHandler::IsGameOpen()
{
    return GetBlackOpsProcess() != NULL;
}

HANDLE GameHandler::GetBlackOpsProcess()
{
    DWORD pId = GetProcessIdByName(L"BlackOps.exe");

    if (pId == 0) {
        pId = GetProcessIdByName(L"BGamerT5.exe");
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
    return hProc;
}

void GameHandler::CloseBlackOps()
{
    if (IsGameOpen())
    {
        HANDLE blackOpsProcess = GetBlackOpsProcess();
        TerminateProcess(blackOpsProcess, 0);
        CloseHandle(blackOpsProcess);
    }
}

string GameHandler::GetZoneCommon()
{
	return GetBlackOpsPath() + "/zone/Common/";
}

string GameHandler::GetBlackOpsPath()
{
	string blackOpsPath = GetPathToExe();
	string::size_type pos = blackOpsPath.find_last_of("\\/");
	return blackOpsPath.substr(0, pos);
}

string GameHandler::GetPathToExe() {
    HANDLE hProcess = GetBlackOpsProcess();

	if (hProcess == NULL) {
		return "";
	}

	wchar_t path[MAX_PATH];
	DWORD size = sizeof(path) / sizeof(path[0]);

	if (QueryFullProcessImageName(hProcess, 0, path, &size)) {
		int length = WideCharToMultiByte(CP_UTF8, 0, path, -1, NULL, 0, NULL, NULL);
		std::string processPath(length, '\0');
		WideCharToMultiByte(CP_UTF8, 0, path, -1, &processPath[0], length, NULL, NULL);
		return processPath;
	}
	else {
		return "";
	}
}

DWORD GameHandler::GetProcessIdByName(const std::wstring& procName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32First(processesSnapshot, &processInfo)) {
        do {
            if (procName == processInfo.szExeFile) {
                CloseHandle(processesSnapshot);
                return processInfo.th32ProcessID;
            }
        } while (Process32Next(processesSnapshot, &processInfo));
    }

    CloseHandle(processesSnapshot);
    return 0;
}