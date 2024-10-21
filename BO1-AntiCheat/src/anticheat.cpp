#include "Constants.h"

#include "game/game_handler.hpp"

#include "game/game_integrity.hpp"

#include "display/display.hpp"

#include "display/statuses.h"

#include "display/verification.hpp"

#include "utilities/memory.hpp"

using namespace std;

bool initialized = false;
bool performed_integrity_check = false;
bool cheats_detected = false;

int last_map_id = 0;
int current_map_id = 0;

vector<string> scannable_maps;
vector<string> cheats_found;

static void init()
{
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

    Verification v;
    v.init();
}

static void NotifyGameClosed()
{
    Display display;
    GameHandler gh;

    display.UpdateStatus(DisplayStatuses::GAME_NOT_CONNECTED);

    initialized = false;
    performed_integrity_check = false;
    gh.OnGameClose();
}

static void AddCheatsFound(string found_cheat)
{
    cheats_detected = true;
    cheats_found.push_back(found_cheat);
}

static void NotifyCheatsDetected()
{
    Display display;
    display.PreUpdateStatus(Constants::CHEATING_DETECTED);

    for (string found_cheat : cheats_found)
    {
        display.AddToStatus("- " + found_cheat);
    }

    display.AddToStatus("\n");
    display.Update();

    GameHandler gl;
    gl.CloseBlackOps();
}

static void WaitForIntegrityCheck()
{
    Display display;
    GameIntegrity gi;
    GameHandler gh;

    last_map_id = current_map_id;
    current_map_id = gh.GetMapId();

    bool map_id_changed = performed_integrity_check && last_map_id != 0 && current_map_id == 0;
    if (map_id_changed)
    {
        performed_integrity_check = false;
    }

    if (!performed_integrity_check)
    {
        bool last_map_id_valid = last_map_id != 0 && last_map_id != -1;
        if (last_map_id_valid && current_map_id == 0)
        {
            display.UpdateStatus(DisplayStatuses::PERFORMING_SCANS);
            performed_integrity_check = true;
            Sleep(1000);

            if (gi.IsStealthPatchDLLPresent())
            {
                AddCheatsFound("A known stealth patch DLL was injected.");
            }

            if (gh.IsGameModLoaded() && gh.IsModLoaded())
            {
                if (!gi.IsModFileValid())
                {
                    AddCheatsFound("mod.ff was found to be modified.");
                }
            }

            for (string map : scannable_maps)
            {
                if (!gi.IsFastfilePatchValid(map))
                {
                    AddCheatsFound(map + "_patch.ff was found to be modified.");
                }
            }

            if (gi.DoExtraFilesExist())
            {
                AddCheatsFound("Extra files found in zone/Common, could be a stealth patch.");
            }

            if (cheats_detected)
            {
                NotifyCheatsDetected();
                return;
            }

            display.UpdateStatus(DisplayStatuses::NO_CHEATING_DETECTED);
        }
    }
    
    int main_menu_id = 5;
    if (gh.GetMapId() == main_menu_id)
    {
        display.UpdateStatus(DisplayStatuses::WAITING_FOR_MAP_LOAD);
        performed_integrity_check = false;
    }
}

int main()
{
    SetConsoleTitle(DisplayStatuses::TITLE);
    init();
    
    GameIntegrity gi;
    gi.init();

    GameHandler gh;
    gh.CheckIfOpen();

    Display display;
    display.Update();

    while (true)
    {
        if (cheats_detected)
        {
            continue;
        }

        if (!gh.IsGameOpen())
        {
            NotifyGameClosed();
            display.UpdateStatus(DisplayStatuses::GAME_NOT_CONNECTED);
            continue;
        }

        if (!gh.GameAlreadyOpen())
        {
            display.UpdateStatus(DisplayStatuses::GAME_ALREADY_OPEN);
            continue;
        }

        if (!initialized)
        {
            display.UpdateStatus(DisplayStatuses::GAME_CONNECTED);
            initialized = true;
        }

        WaitForIntegrityCheck();
        Sleep(1000);
    }

    return 0;
}