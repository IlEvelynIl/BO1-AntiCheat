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

map<int, string> maps;

static void init()
{
    // map ids to map codename
    maps[3] = "zombie_cod5_prototype";
    maps[37] = "zombie_theater";
    maps[77] = "zombie_pentagon";
    maps[50] = "zombie_cod5_asylum";
    maps[85] = "zombie_coast";
    maps[90] = "zombie_cod5_factory";
    maps[99] = "zombie_cod5_sumpf";
    maps[129] = "zombie_temple";
    maps[163] = "zombie_moon";
    maps[189] = "zombie_cosmodrome";

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
            Sleep(1000); // Wait to scan, so that it puts it during the map load

            if (!fastfiles.CommonZombiePatchValid())
            {
                onDetected("common_zombie_patch.ff was modified.");
                return;
            }

            for (auto const& [key, val] : maps)
            {
                string map = val;
                if (!fastfiles.MapFastFileValid(map))
                {
                    onDetected("One of the map patches was modified.");
                    return;
                }
            }

            if (fastfiles.ExtraFilesExist())
            {
                onDetected("Extra files found in zone/Common, could be a stealth patch.");
                return;
            }

            display.UpdateStatus(DisplayStatuses::NO_CHEATING_DETECTED);
        }
    }
    else if (gh.GetMapId() == 5) // main menu
    {
        display.UpdateStatus(DisplayStatuses::GAME_CONNECTED_WAITING);
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
            display.UpdateStatus(DisplayStatuses::GAME_CONNECTED_WAITING);
            initialized = true;
        }

        tick();
        Sleep(1000); // 1 second sleep so we don't slow down systems.
    }

    return 0;
}