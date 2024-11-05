#include <SFML/Graphics.hpp>

#include "../constants.h"

#include "../game/game.hpp"

#include "integrity/integrity.hpp"

#include "../statuses.h"

#include "verification/verification.hpp"

#include "../utils/memory.hpp"

#include "updater/updater.hpp"

using namespace std;

bool initialized = false;
bool performed_integrity_check = false;
bool cheating_detected = false;
bool notified_cheats_detected = false;
bool integrity_check_override = false;

int last_map_id = 0;
int current_map_id = 0;

vector<std::string> scannable_maps;
vector<std::string> cheats_found;

std::string main_status = Statuses::GAME_NOT_CONNECTED;
std::string info_status = "";

namespace anticheat {
    void Initialize()
    {
        // adds the maps that need to be scanned every map launch/quit
        scannable_maps.push_back("frontend");
        scannable_maps.push_back("common_zombie");
        scannable_maps.push_back("zombie_theater");
        scannable_maps.push_back("zombie_pentagon");
        scannable_maps.push_back("zombietron");
        scannable_maps.push_back("zombie_cosmodrome");
        scannable_maps.push_back("zombie_coast");
        scannable_maps.push_back("zombie_temple");
        scannable_maps.push_back("zombie_moon");
        scannable_maps.push_back("zombie_cod5_prototype");
        scannable_maps.push_back("zombie_cod5_asylum");
        scannable_maps.push_back("zombie_cod5_sumpf");
        scannable_maps.push_back("zombie_cod5_factory");

        // initialize the verification uids
        verification::CalculateUIDs();

        // initialize the hashes for each fastfile
        integrity::SetupIntegrityHashes();
    }

    // displays the "Game not connected." message
    void OnGameClosed()
    {
        main_status = Statuses::GAME_NOT_CONNECTED;
        info_status = "";
        initialized = false;
        performed_integrity_check = false;
    }

    // specifically makes it so that we dont have to restart the anti cheat when cheats were previously detected
    void OnGameReopen()
    {
        notified_cheats_detected = false;
        if (cheating_detected)
        {
            cheating_detected = false;
        }
        initialized = false;
        cheats_found.clear();
        info_status = "";
    }

    // adds a cheating method to a list, this will be shown in a second window
    void OnCheatFound(string cheating_method)
    {
        cheats_found.push_back(cheating_method);
    }

    // crashes bo1 when a cheating method is found,
    // also pops up the second window that explains what was found
    // this is especially good for players who may accidentally leave something in their files
    void NotifyCheatsDetected()
    {
        cheating_detected = true;
        main_status = Statuses::CHEATING_DETECTED;
        info_status = Statuses::MORE_INFO_WINDOW;
        notified_cheats_detected = true;
        game::CloseBlackOps();

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
        last_map_id = current_map_id;
        current_map_id = game::GetMapId();

        bool map_id_changed = performed_integrity_check && last_map_id != 0 && current_map_id == 0;
        if (map_id_changed)
        {
            performed_integrity_check = false;
        }

        // only check when the map is being loaded/quit
        if (!performed_integrity_check)
        {
            bool last_map_id_valid = last_map_id != 0 && last_map_id != -1;
            if ((last_map_id_valid && current_map_id == 0) || integrity_check_override)
            {
                main_status = Statuses::CHECKING_FOR_PATCHES;
                info_status = "This may take a moment";

                integrity_check_override = false;
                performed_integrity_check = true;
                Sleep(1000);

                // check for any extra files, they should not be there
                if (integrity::DoExtraFilesExist())
                {
                    OnCheatFound("Extra files found in zone/Common, could be a stealth patch.");
                }

                // check for any known stealth patch injections
                if (integrity::IsStealthPatchDLLPresent())
                {
                    OnCheatFound("A known stealth patch DLL was injected.");
                }

                // community leaderboard mods: check hashes of the currently loaded mod
                if (game::IsGameModLoaded() && game::IsModLoaded())
                {
                    if (!integrity::IsModFileValid())
                    {
                        OnCheatFound("mod.ff was found to be modified.");
                    }
                }

                // check every single fastfile patches
                for (string map : scannable_maps)
                {
                    if (!integrity::IsFastfilePatchValid(map))
                    {
                        OnCheatFound(map + "_patch.ff was found to be modified.");
                    }
                }

                // if theres any cheats detected, notify them and crash bo1
                if (cheats_found.size() > 0)
                {
                    NotifyCheatsDetected();
                }
                else // otherwise tell them they're good, but we will also tell them that the anti cheat is still gonna check during the game
                {
                    main_status = Statuses::NO_PATCHING_DETECTED;
                    info_status = Statuses::WILL_CONTINUE_SEARCH;
                }
            }
        }

        // check game values such as godmode, box movable, etc.
        int map_id = game::GetMapId();
        if (map_id != -1 && map_id != 0)
        {
            string detectedBinds = integrity::LookForActiveCheatingBinds();
            if (detectedBinds != "")
            {
                OnCheatFound(detectedBinds);
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
        if (!game::IsGameOpen() && !cheating_detected)
        {
            OnGameClosed();
            main_status = Statuses::GAME_NOT_CONNECTED;
            info_status = "";
            game::CheckGameMod();
            return;
        }

        // we want to be able to reuse the tool after cheats are detected
        if (notified_cheats_detected && game::IsGameOpen())
        {
            OnGameReopen();
        }

        // this checks for certain things once, think of it as an "OnGameOpen"
        if (!initialized)
        {
            int map_id = game::GetMapId();
            if (map_id != Constants::MAIN_MENU_ID)
            {
                if (map_id != 0 && map_id != -1 && info_status != Statuses::MORE_INFO_WINDOW)
                {
                    info_status = Statuses::PATCHES_NOT_VERIFIED;
                }
                else {
                    info_status = "";
                }
                main_status = Statuses::GAME_CONNECTED;
            }

            game::CheckGameMod();
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