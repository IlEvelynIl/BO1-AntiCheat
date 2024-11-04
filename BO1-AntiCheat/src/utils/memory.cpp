#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>

#include "memory.hpp"

using namespace std;

namespace memory {
    int ReadInt(HANDLE processHandle, int address) {
        if (address == -1)
        {
            return -1;
        }

        int buffer = 0;
        SIZE_T NumberOfBytesToRead = sizeof(buffer);
        SIZE_T NumberOfBytesActuallyRead;
        bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);

        if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
        {
            return -1;
        }

        return buffer;
    }

    bool WriteInt(HANDLE processHandle, int address, int value)
    {
        if (address == -1)
        {
            return false;
        }

        bool success = WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(value), NULL);
        if (!success) {
            return false;
        }

        return true;
    }

    char* ReadString(HANDLE processHandle, int address) {
        std::string text = "";

        if (address == -1) {
            return &text[0];
        }

        char buffer = !0;
        char* stringToRead = new char[128];
        SIZE_T NumberOfBytesToRead = sizeof(buffer);
        SIZE_T NumberOfBytesActuallyRead;
        int i = 0;

        while (buffer != 0) {
            bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, NumberOfBytesToRead, &NumberOfBytesActuallyRead);
            if (!success || NumberOfBytesActuallyRead != NumberOfBytesToRead)
            {
                return &text[0];
            }

            stringToRead[i] = buffer;
            i++;
            address++;
        }

        return stringToRead;
    }

    bool WriteString(HANDLE processHandle, int address, string value)
    {
        if (address == -1)
        {
            return false;
        }

        int length = value.length() + 1;
        bool success = WriteProcessMemory(processHandle, (LPVOID)address, value.c_str(), length, NULL);

        if (!success) {
            return false;
        }

        return true;
    }
}