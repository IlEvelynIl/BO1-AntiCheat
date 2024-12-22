#include <SFML/Graphics.hpp>

#include "../constants.h"

#include "../game/game.hpp"

#include "../game/process.hpp"

#include "integrity/integrity.hpp"

#include "../statuses.h"

#include "verification/verification.hpp"

#include "../utils/memory.hpp"

#include "updater/updater.hpp"

#include "integrity/config.hpp"

using namespace std;

bool initialized = false;
bool performed_integrity_check = false;
bool cheating_detected = false;
bool notified_cheats_detected = false;
bool integrity_check_override = false;

int last_map_id = 0;
int current_map_id = 0;

vector<std::string> cheats_found;

std::string main_status = Statuses::GAME_NOT_CONNECTED;
std::string info_status = "";

namespace anticheat {
    void Initialize()
    {
        // initialize the verification uids
        verification::CalculateUIDs();

        // initialize integrity checks
        integrity::Initialize();

        // set which config binds are not allowed
        integrity::config::Initialize();
    }

    // displays the "Game not connected." message
    void OnGameClosed()
    {
        main_status = Statuses::GAME_NOT_CONNECTED;
        info_status = "";
        initialized = false;
        performed_integrity_check = false;
    }

    // adds a cheating method to a list, this will be shown in a second window
    void AddCheatFound(string cheating_method)
    {
        cheats_found.push_back(cheating_method);
    }

    // pops up the second window that explains what was found
    // this is especially good for players who may accidentally leave something in their files
    void NotifyCheatsDetected()
    {
        cheating_detected = true;

        // notify the player
        main_status = Statuses::CHEATING_DETECTED;
        info_status = Statuses::MORE_INFO_WINDOW;
        notified_cheats_detected = true;

        string cheats = "The following cheating methods were detected:\n";
        for (string cheat_found : cheats_found)
        {
            cheats += "\n- " + cheat_found;
        }

        MessageBoxA(NULL, cheats.c_str(), "BO1 Anti Cheat (Detections)", MB_OK);
    }

    // this is the magic of the tool, handles all checks performed to ensure a bo1 game is safe
    void AttemptIntegrityCheck()
    {
        if (!game::process::IsGameOpen() || cheating_detected)
        {
            return;
        }

        last_map_id = current_map_id;
        current_map_id = game::GetMapId();

        bool map_id_changed = performed_integrity_check && last_map_id != 0 && current_map_id == 0;
        if (map_id_changed)
        {
            performed_integrity_check = false;
        }

        bool ff_checking_after_map_load = integrity_check_override && info_status == Statuses::PATCHES_CHECKED_AFTER_MAP_LOAD;

        // only check when the map is being loaded/quit
        if (!performed_integrity_check)
        {
            bool last_map_id_valid = last_map_id != 0 && last_map_id != -1;
            if ((last_map_id_valid && current_map_id == 0) || integrity_check_override)
            {
                main_status = Statuses::CHECKING_FOR_PATCHES;
                info_status = "This may take a moment";

                verification::CalculateUIDs();

                integrity_check_override = false;
                performed_integrity_check = true;

                Sleep(1000); // puts us in the loading screen so they cant edit the files

                // check for any extra files, they should not be there
                string extra_zone_files = integrity::GetExtraFilesInZone();
                if (extra_zone_files != "")
                {
                    AddCheatFound("Extra files found in zone, could be a stealth patch: " + extra_zone_files);
                }

                // check for any known stealth patch injections
                if (integrity::IsStealthPatchInjected())
                {
                    AddCheatFound("A known stealth patch DLL was injected.");
                }

                // community leaderboard mods: check hashes of the currently loaded mod
                if (game::IsGameModLoaded() && game::IsModLoaded())
                {
                    if (!integrity::IsModFileValid())
                    {
                        AddCheatFound("mod.ff was found to be modified.");
                    }
                }

                // list off modified common files
                string modified_fastfiles = integrity::GetModifiedFastfiles();
                if (modified_fastfiles != "")
                {
                    AddCheatFound("Modified fastfiles: " + modified_fastfiles);
                }

                // list off modified lang files
                string modified_lang_files = integrity::GetModifiedLangFiles();
                if (modified_lang_files != "")
                {
                    AddCheatFound("Modified lang files: " + modified_lang_files);
                }

                // if theres any cheats detected, notify them and crash bo1
                if (cheats_found.size() > 0)
                {
                    NotifyCheatsDetected();
                }
                else // otherwise tell them they're good, but we will also tell them that the anti cheat is still gonna check during the game
                {
                    main_status = Statuses::NO_PATCHING_DETECTED;
                    if (!ff_checking_after_map_load)
                    {
                        info_status = Statuses::WILL_CONTINUE_SEARCH;
                    }
                    else
                    {
                        info_status = Statuses::PATCHES_CHECKED_AFTER_MAP_LOAD;
                    }
                }
            }
        }

        // check game values such as godmode, box movable, etc.
        int map_id = game::GetMapId();
        if (map_id != Constants::MAIN_MENU_ID && map_id != -1 && map_id != 0)
        {
            string playerStates = integrity::GetModifiedPlayerStates();
            if (playerStates != "")
            {
                AddCheatFound(playerStates);
                NotifyCheatsDetected();
                return;
            }

            // check config but only if its been modified
            string cheatingCommands = integrity::config::GetCheatingCommands();
            if (cheatingCommands != "")
            {
                AddCheatFound("Cheating commands found in the config: " + cheatingCommands);
                NotifyCheatsDetected();
                return;
            }
        }

        if (game::GetMapId() == Constants::MAIN_MENU_ID)
        {
            main_status = Statuses::GAME_CONNECTED;
            info_status = Statuses::WAITING_FOR_MAP_LOAD_QUIT;
            performed_integrity_check = false;
        }
    }

    // waits for the game to be opened before we run any checks
    void WaitForBlackOpsProcess()
    {
        if (!game::process::IsGameOpen() && !cheating_detected)
        {
            OnGameClosed();
            main_status = Statuses::GAME_NOT_CONNECTED;
            info_status = "";
            game::CheckForAllowedTools();
            return;
        }

        // this checks for certain things once, think of it as an "OnGameOpen"
        if (!initialized)
        {
            int map_id = game::GetMapId();
            if (map_id != Constants::MAIN_MENU_ID)
            {
                info_status = "";
                main_status = Statuses::GAME_CONNECTED;

                // check for patching methods mid game
                if (map_id != 0 && map_id != -1)
                {
                    integrity_check_override = true;
                    info_status = Statuses::PATCHES_CHECKED_AFTER_MAP_LOAD;
                }
            }

            game::CheckForAllowedTools();
            initialized = true;
        }

        AttemptIntegrityCheck();
        Sleep(50);
    }

    bool CheatingDetected()
    {
        return cheating_detected;
    }

    std::string GetMainStatus()
    {
        return main_status;
    }

    std::string GetInfoStatus()
    {
        return info_status;
    }
}