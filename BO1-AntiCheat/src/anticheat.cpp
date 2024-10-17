#include "Constants.h"

#include "game/game_handler.hpp"

#include "game/game_integrity.hpp"

#include "display/display.hpp"

#include "display/statuses.h"

#include "display/verification.hpp"

#include "utilities/memory.hpp"

using namespace std;

bool initialized = false;
bool performed_scans = false;
bool cheating = false;

int last_map_id = 0;
int current_map_id = 0;

vector<string> scannable_maps;

static void init()
{
    // define all bo1 map codenames
    scannable_maps.push_back("zombie_theater");
    scannable_maps.push_back("zombie_pentagon");
    scannable_maps.push_back("zombie_cosmodrome");
    scannable_maps.push_back("zombie_coast");
    scannable_maps.push_back("zombie_temple");
    scannable_maps.push_back("zombie_moon");
    scannable_maps.push_back("zombie_cod5_prototype");
    scannable_maps.push_back("zombie_cod5_asylum");
    scannable_maps.push_back("zombie_cod5_sumpf");
    scannable_maps.push_back("zombie_cod5_factory");

    // initialize verification method
    Verification v;
    v.init();
}

static void reset()
{
    Display display;
    GameHandler gh;

    display.UpdateStatus(DisplayStatuses::GAME_NOT_CONNECTED);

    initialized = false;
    performed_scans = false;
    gh.OnGameClose();
}

static void onDetected(const char* reason)
{
    cheating = true;

    Display display;
    GameHandler gl;

    string combinedStatus = string(DisplayStatuses::CHEATING_DETECTED) + "\nReason: " + reason;
    display.UpdateStatus(combinedStatus.c_str());

    gl.CloseBlackOps();
}

static void tick()
{
    Display display;
    GameIntegrity gi;
    GameHandler gh;

    last_map_id = current_map_id;
    current_map_id = gh.GetMapId();

    // Ensures to rescan when a map bind is used
    if (performed_scans && last_map_id != 0 && current_map_id == 0)
    {
        performed_scans = false;
    }

    if (!performed_scans)
    {
        bool lastMapIdValid = last_map_id != 0 && last_map_id != -1;
        if (lastMapIdValid && current_map_id == 0)
        {
            display.UpdateStatus(DisplayStatuses::PERFORMING_SCANS);
            performed_scans = true;
            Sleep(1000); // we wait to scan as this puts us during the map load (fastfile extraction, script compilation, etc.)

            // check the common zombie patch
            if (!gi.CommonZombiePatchValid())
            {
                onDetected("common_zombie_patch.ff was modified.");
                return;
            }

            // check every zombies fastfile patch
            for (string map : scannable_maps)
            {
                if (!gi.MapFastFileValid(map))
                {
                    onDetected("One of the map fastfiles were modified.");
                    return;
                }
            }

            // prevent frontend_patch cheating
            if (!gi.Game_ModFrontendPatchValid())
            {
                onDetected("frontend_patch.ff was modified.");
                return;
            }

            // check for anything that shouldnt be there
            if (gi.ExtraFilesExist())
            {
                onDetected("Extra files found in zone/Common, could be a stealth patch.");
                return;
            }

            display.UpdateStatus(DisplayStatuses::NO_CHEATING_DETECTED);
        }
    }
    
    // reset scan toggle on the main menu
    if (gh.GetMapId() == 5)
    {
        display.UpdateStatus(DisplayStatuses::WAITING_FOR_MAP_LOAD);
        performed_scans = false;
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
    display.UpdateDisplay();

    while (true)
    {
        if (cheating)
        {
            continue;
        }

        if (!gh.IsGameOpen())
        {
            reset();
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

        tick();
        Sleep(1000); // 1 second sleep so we don't slow down systems.
    }

    return 0;
}