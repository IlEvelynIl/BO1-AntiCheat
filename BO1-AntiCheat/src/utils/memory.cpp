#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <psapi.h>

#include "memory.hpp"

using namespace std;

namespace utils {
    namespace memory {
        int ReadInt(HANDLE processHandle, int address) {
            if (address == -1 || processHandle == nullptr) {
                return -1;
            }

            int buffer = 0;
            SIZE_T bytesRead = 0;
            bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(buffer), &bytesRead);

            if (!success || bytesRead != sizeof(buffer)) {
                return -1;
            }

            return buffer;
        }

        std::string ReadString(HANDLE processHandle, int address) {
            if (address == -1 || processHandle == nullptr) {
                return "";
            }

            char buffer = 0;
            string result;
            SIZE_T bytesRead = 0;

            // Read string byte-by-byte until null terminator
            while (true) {
                bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(buffer), &bytesRead);
                if (!success || bytesRead != sizeof(buffer) || buffer == 0) {
                    break;
                }

                result.push_back(buffer);
                address++;
            }

            return result;
        }
    } // memory
} // utils