#include "process.hpp"

#include <windows.h>

#include <string>

#include <TlHelp32.h>

#include <psapi.h>

namespace game {
	namespace process {
        // gets the bo1 process so we can utilize it
        HANDLE GetBlackOpsProcess()
        {
            DWORD pId = GetProcessIdByName(L"BlackOps.exe");

            if (pId == 0) {
                pId = GetProcessIdByName(L"BGamerT5.exe");
            }

            HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
            return hProc;
        }

        // checks if the game is open
        bool IsGameOpen()
        {
            HANDLE hProcess = GetBlackOpsProcess();
            if (hProcess != NULL) {
                CloseHandle(hProcess);
                return true;
            }
            return false;
        }

        // gets the path to the actual bo1 executable, with the .exe name included
        std::string GetPathToExe() {
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

        // finds a process id by name
        DWORD GetProcessIdByName(const std::wstring& procName)
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
	} // process
} // game