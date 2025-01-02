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

#include "../utils/files.hpp"

using namespace std;

bool game_mod_loaded = false;
bool custom_fx_loaded = false;

namespace fs = std::filesystem;

namespace game {

    // reads the map id from memory, Furret's box tracker uses this same method for setting up the weapons list
    int GetMapId()
    {
        HANDLE handle = process::GetBlackOpsProcess();
        if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
            return -1;
        }

        int mapId = utils::memory::ReadInt(handle, Constants::C_MAPADDRESS);
        return mapId;
    }

    // gets the total game time
    int GetGameTime()
    {
        HANDLE handle = process::GetBlackOpsProcess();
        if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
            return -1;
        }

        int gameTime = utils::memory::ReadInt(handle, Constants::C_TIMEADDRESS);
        return gameTime;
    }

    // gets the path to zone/Common
    std::string GetZoneCommon()
    {
        return GetBlackOpsPath() + "\\zone\\Common\\";
    }

    // gets the path to the current langauge folder
    std::string GetZoneLanguage()
    {
        return GetBlackOpsPath() + "\\zone\\" + GetLanguageFolder() + "\\";
    }

    // gets the path to the executable thats being run
    std::string GetBlackOpsPath()
    {
        std::string blackOpsPath = process::GetPathToExe();
        std::string::size_type pos = blackOpsPath.find_last_of("\\/");
        return blackOpsPath.substr(0, pos);
    }

    // acts as an initializer for the game_mod_loaded boolean
    void CheckForAllowedTools()
    {
        game_mod_loaded = IsGameModPresent();
        custom_fx_loaded = IsCustomFxToolPresent();
    }

    // just a getter for the value of game_mod_loaded
    bool IsGameModLoaded()
    {
        return game_mod_loaded;
    }

    // gets the current mod name from memory
    std::string GetModName()
    {
        if (!game_mod_loaded) {
            return "";
        }

        HANDLE handle = process::GetBlackOpsProcess();
        if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
            return "";
        }

        std::string modName = utils::memory::ReadString(handle, Constants::C_MODADDRESS);
        return modName;
    }

    // simple check for if a mod is loaded or not
    bool IsModLoaded()
    {
        if (!IsGameModLoaded())
        {
            return false;
        }
        return GetModName() != "";
    }

    // checks for the game_mod.dll file in the modules list of bo1
    bool IsGameModPresent()
    {
        HANDLE handle = process::GetBlackOpsProcess();
        if (!process::IsGameOpen()) {
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
                        if (utils::files::GetMD5(dllPath) == Checksums::GAME_MOD_DLL)
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    // this checks if the custom fx tool is present in the game
    bool IsCustomFxToolPresent()
    {
        HANDLE handle = process::GetBlackOpsProcess();
        if (!process::IsGameOpen()) {
            return false;
        }

        string game_folder = game::GetBlackOpsPath();
        string steam_api = game_folder + "\\steam_api.dll";
        string fx_tool = game_folder + "\\blackops-fx.dll";

        // check for a match on both steam_api.dll and blackops-fx.dll
        // every install of black ops requires a steam_api.dll file
        // so we can just check those hashes and see if they match up
        if (fs::exists(steam_api) && fs::exists(fx_tool))
        {
            string steam_api_hash = utils::files::GetMD5(steam_api);
            string fx_dll_hash = utils::files::GetMD5(fx_tool);

            if (steam_api_hash == Checksums::CUSTOM_FX_STEAM_API_DLL && fx_dll_hash == Checksums::CUSTOM_FX_DLL)
            {
                return true;
            }
        }

        return false;
    }

    bool IsCustomFxToolLoaded()
    {
        return custom_fx_loaded;
    }

    std::string GetLanguageFolder()
    {
        string lang = GetGameLanguage();

        if (lang == "english")
        {
            return "English";
        }
        else if (lang == "japanese")
        {
            return "Japanese";
        }
        else if (lang == "french")
        {
            return "French";
        }
        else if (lang == "austrian")
        {
            return "German";
        }
        else if (lang == "italian")
        {
            return "Italian";
        }
        else if (lang == "spanish")
        {
            return "Spanish";
        }

        return "English";
    }

    std::string GetGameLanguage()
    {
        HANDLE handle = process::GetBlackOpsProcess();
        if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
            return "";
        }

        std::string lang = utils::memory::ReadString(handle, Constants::C_LANGADDRESS);
        return lang;
    }
}