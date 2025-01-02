#include "process.hpp"

#include <windows.h>
#include <string>
#include <TlHelp32.h>
#include <psapi.h>

HANDLE hGameProcess = NULL;

namespace game {
    namespace process {
        // gets the bo1 process so we can utilize it
        HANDLE GetBlackOpsProcess()
        {
            if (hGameProcess != NULL)
            {
                DWORD exitCode = 0;
                if (GetExitCodeProcess(hGameProcess, &exitCode) && exitCode == STILL_ACTIVE)
                {
                    return hGameProcess;
                }

                // If the cached process handle is invalid, clean it up
                CloseHandle(hGameProcess);
                hGameProcess = NULL;
            }

            DWORD pId = GetProcessIdByName(L"BlackOps.exe");
            if (pId == 0)
            {
                pId = GetProcessIdByName(L"BGamerT5.exe");
            }

            if (pId != 0)
            {
                hGameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
            }

            return hGameProcess;
        }

        // checks if the game is open
        bool IsGameOpen()
        {
            HANDLE handle = GetBlackOpsProcess();
            return handle != NULL && handle != INVALID_HANDLE_VALUE;
        }

        // gets the path to the actual bo1 executable, with the .exe name included
        std::string GetPathToExe() {
            HANDLE handle = GetBlackOpsProcess();

            if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
                return "";
            }

            wchar_t path[MAX_PATH];
            DWORD size = MAX_PATH;

            std::string processPath;
            if (QueryFullProcessImageName(handle, 0, path, &size)) {
                int length = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
                if (length > 0) {
                    processPath.resize(length - 1);
                    WideCharToMultiByte(CP_UTF8, 0, path, -1, processPath.data(), length, nullptr, nullptr);
                }
            }

            return processPath;
        }

        // finds a process id by name
        DWORD GetProcessIdByName(const std::wstring& procName)
        {
            PROCESSENTRY32 processInfo;
            processInfo.dwSize = sizeof(processInfo);

            HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
            if (processesSnapshot == NULL || processesSnapshot == INVALID_HANDLE_VALUE) {
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