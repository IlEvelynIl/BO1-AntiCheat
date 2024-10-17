#include "Constants.h"

#include "game/game_handler.hpp"

#include "game/fastfiles.hpp"

#include "display/display.hpp"

#include "display/statuses.h"

#include "display/verification.hpp"

#include "utilities/memory.hpp"

using namespace std;

bool initialized = false;
bool performedScans = false;
bool cheating = false;

int lastMapId = 0;
int latestMapId = 0;

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

    Verification v;
    v.init();
}

static void reset()
{
    Display display;
    GameHandler gh;

    display.UpdateStatus(DisplayStatuses::GAME_NOT_CONNECTED);

    initialized = false;
    performedScans = false;
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
    FastFiles fastfiles;
    GameHandler gh;

    lastMapId = latestMapId;
    latestMapId = gh.GetMapId();

    // Ensures to rescan when a map bind is used
    if (performedScans && lastMapId != 0 && latestMapId == 0)
    {
        performedScans = false;
    }

    if (!performedScans)
    {
        bool lastMapIdValid = lastMapId != 0 && lastMapId != -1;
        if (lastMapIdValid && latestMapId == 0)
        {
            display.UpdateStatus(DisplayStatuses::PERFORMING_SCANS);
            performedScans = true;
            Sleep(1000); // Wait to scan, this puts us during the map load where the fastfiles are extracted

            // check the common zombie patch
            if (!fastfiles.CommonZombiePatchValid())
            {
                onDetected("common_zombie_patch.ff was modified.");
                return;
            }

            // check every single "_patch" fastfile
            for (string map : scannable_maps)
            {
                if (!fastfiles.MapFastFileValid(map))
                {
                    onDetected("One of the map fastfiles were modified.");
                    return;
                }
            }

            // prevent frontend_patch cheating
            if (!fastfiles.Game_ModFrontendPatchValid())
            {
                onDetected("frontend_patch.ff was modified.");
                return;
            }

            // check for anything that shouldnt be there
            if (fastfiles.ExtraFilesExist())
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
        performedScans = false;
    }
}

int main()
{
    SetConsoleTitle(DisplayStatuses::TITLE);
    init();
    
    FastFiles ffHandler;
    ffHandler.init();

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