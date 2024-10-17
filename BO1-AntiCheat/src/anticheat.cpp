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
bool cheating = false;

int last_map_id = 0;
int current_map_id = 0;

vector<string> scannable_maps;

static void init()
{
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

static void reset()
{
    Display display;
    GameHandler gh;

    display.UpdateStatus(DisplayStatuses::GAME_NOT_CONNECTED);

    initialized = false;
    performed_integrity_check = false;
    gh.OnGameClose();
}

static void onDetected(const char* reason)
{
    cheating = true;

    Display display;
    GameHandler gl;

    string combined_status = string(DisplayStatuses::CHEATING_DETECTED) + "\nReason: " + reason;
    display.UpdateStatus(combined_status.c_str());

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
            Sleep(1000); // this puts us during the map load (fastfile extraction, script compilation, etc.)

            if (gi.IsStealthPatchDLLPresent())
            {
                onDetected("A known stealth patch DLL was injected.");
                return;
            }

            if (!gi.IsCommonZombiePatchValid())
            {
                onDetected("common_zombie_patch.ff was modified.");
                return;
            }

            for (string map : scannable_maps)
            {
                if (!gi.IsMapFastFileValid(map))
                {
                    onDetected("One of the map fastfiles were modified.");
                    return;
                }
            }

            if (!gi.IsGameModFrontendPatchValid())
            {
                onDetected("frontend_patch.ff was modified.");
                return;
            }

            if (gi.DoExtraFilesExist())
            {
                onDetected("Extra files found in zone/Common, could be a stealth patch.");
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

        WaitForIntegrityCheck();
        Sleep(1000);
    }

    return 0;
}