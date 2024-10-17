#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>
#include <map>

#include "game_handler.hpp"
#include "../utilities/memory.hpp"
#include "../constants.h"

using namespace std;

bool sessionValid = true;
bool hasCheckedSession = false;

int GameHandler::GetMapId()
{
    Memory mem;
    return mem.ReadInt(GetBlackOpsProcess(), Constants::C_MAPADDRESS);
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

void GameHandler::CheckIfOpen()
{
    if (IsGameOpen())
    {
        sessionValid = false;
    }
}

void GameHandler::OnGameClose()
{
    sessionValid = true;
}

bool GameHandler::GameAlreadyOpen()
{
    return sessionValid;
}