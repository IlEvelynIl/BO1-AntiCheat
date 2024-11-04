#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>

#include "memory.hpp"

using namespace std;

BOOL Memory::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
        //printf("LookupPrivilegeValue error: %u\n", GetLastError() );
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) {
        //printf("AdjustTokenPrivileges error: %u\n", GetLastError() );
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        //printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}
BOOL Memory::GetDebugPrivileges(void) {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
        return FALSE; //cout << "OpenProcessToken() failed, error\n>> " << GetLastError() << std::endl;
    //else cout << "OpenProcessToken() is OK, got the handle!" << std::endl;

    if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
        return FALSE; //cout << "Failed to enable privilege, error:\n>> " << GetLastError() << std::endl;

    return TRUE;
}

int Memory::ReadInt(HANDLE processHandle, int address) {
    if (address == -1)
        return -1;
    int buffer = 0;
    SIZE_T NumberOfBytesToRead = sizeof(buffer); //this is equal to 4
    SIZE_T NumberOfBytesActuallyRead;
    BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
    if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead) {
        //cout << "Memory Error!" << std::endl;
        return -1;
    }
    //if (err || NumberOfBytesActuallyRead != NumberOfBytesToRead) {
    //	DWORD lastError = GetLastError();
    //	if (lastError != 0)
    //        cout << lastError << std::endl;
    //    cout << "blub" << std::endl;
    //}
    return buffer;
}

bool Memory::WriteInt(HANDLE processHandle, int address, int value)
{
    if (address == -1)
        return false;

    BOOL success = WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(value), NULL);
    if (!success) {
        //cout << "Memory Write Error!" << std::endl;
        return false;
    }
    return true;
}

int Memory::GetPointerAddress(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    int ptr = ReadInt(processHandle, startAddress);
    for (int i = 0; i < offsetCount - 1; i++) {
        ptr += offsets[i];
        ptr = ReadInt(processHandle, ptr);
    }
    ptr += offsets[offsetCount - 1];
    return ptr;
}

int Memory::ReadPointerInt(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    return ReadInt(processHandle, GetPointerAddress(processHandle, startAddress, offsets, offsetCount));
}

float Memory::ReadFloat(HANDLE processHandle, int address) {
    if (address == -1)
        return -1;
    float buffer = 0.0;
    SIZE_T NumberOfBytesToRead = sizeof(buffer); //this is equal to 4
    SIZE_T NumberOfBytesActuallyRead;
    BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
    if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
        return -1;
    return buffer;
}

float Memory::ReadPointerFloat(HANDLE processHandle, int startAddress, int offsets[], int offsetCount) {
    if (startAddress == -1)
        return -1;
    return ReadFloat(processHandle, GetPointerAddress(processHandle, startAddress, offsets, offsetCount));
}

char* Memory::ReadString(HANDLE processHandle, int address) {
    std::string text = "-1";

    if (address == -1)
        return &text[0];
    char buffer = !0;
    char* stringToRead = new char[128];
    SIZE_T NumberOfBytesToRead = sizeof(buffer);
    SIZE_T NumberOfBytesActuallyRead;
    int i = 0;
    while (buffer != 0) {
        BOOL success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
        if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
            return &text[0];
        stringToRead[i] = buffer;
        i++;
        address++;
    }
    return stringToRead;
}

bool Memory::WriteString(HANDLE processHandle, int address, string value)
{
    if (address == -1)
        return false;

    int length = value.length() + 1;

    BOOL success = WriteProcessMemory(processHandle, (LPVOID)address, value.c_str(), length, NULL);
    if (!success) {
        //cout << "Memory Write Error!" << std::endl;
        return false;
    }
    return true;
}