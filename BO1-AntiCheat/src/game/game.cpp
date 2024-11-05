#include <Windows.h>

#include <TlHelp32.h>

#include <string>

#include <psapi.h>

#include <map>

#include "game.hpp"

#include "process.hpp"

#include "../anticheat/integrity/integrity.hpp"

#include "../anticheat/integrity/checksums.h"

#include "../utils/memory.hpp"

#include "../constants.h"

#include <vector>

using namespace std;

bool game_mod_loaded = false;

namespace game {

    // reads the map id from memory, Furret's box tracker uses this same method for setting up the weapons list
    int GetMapId()
    {
        return memory::ReadInt(process::GetBlackOpsProcess(), Constants::C_MAPADDRESS);
    }

    // gets the path to zone/Common
    std::string GetZoneCommon()
    {
        return GetBlackOpsPath() + "/zone/Common/";
    }

    // gets the path to the executable thats being run
    std::string GetBlackOpsPath()
    {
        std::string blackOpsPath = process::GetPathToExe();
        std::string::size_type pos = blackOpsPath.find_last_of("\\/");
        return blackOpsPath.substr(0, pos);
    }

    // acts as an initializer for the game_mod_loaded boolean
    void CheckForGameMod()
    {
        game_mod_loaded = IsGameModPresent();
    }

    // just a getter for the value of game_mod_loaded
    bool IsGameModLoaded()
    {
        return game_mod_loaded;
    }

    // gets the current mod name from memory
    std::string GetModName()
    {
        if (!game_mod_loaded)
        {
            return "";
        }

        return memory::ReadString(process::GetBlackOpsProcess(), Constants::C_MODADDRESS);
    }

    // simple check for if a mod is loaded or not
    bool IsModLoaded()
    {
        return GetModName() != "";
    }

    // checks for the game_mod.dll file in the modules list of bo1
    bool IsGameModPresent()
    {
        HANDLE handle = process::GetBlackOpsProcess();

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
                        std::string dllPath = std::string(modulePath);

                        // if we have a match then game mod is present
                        if (anticheat::integrity::GetFileMD5(dllPath) == Checksums::GAME_MOD_DLL)
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }
}