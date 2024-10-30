#include "../game/game_integrity.hpp"

#include "../game/game_handler.hpp"

#include "../constants.h"

#include "../utilities/md5.h"

#include "../utilities/memory.hpp"

#include <fstream>

#include <sstream>

using namespace std;

map<string, string> fastfile_hashes;
map<string, string> mod_fastfile_hashes;
vector<string> valid_common_files;
vector<string> stealth_patch_hashes;

// initializes everything that we need to check on the game, such as hashes of fastfiles, stealth patch dll hashes, etc.
void GameIntegrity::init()
{

    fastfile_hashes["frontend_patch.ff"] = Constants::GAME_MOD_FRONTEND;
    fastfile_hashes["common_zombie_patch.ff"] = Constants::COMMON_ZOMBIE_PATCH;
    fastfile_hashes["zombie_theater_patch.ff"] = Constants::ZOMBIE_THEATER_PATCH;
    fastfile_hashes["zombie_theater_patch.ff"] = Constants::ZOMBIE_THEATER_PATCH;
    fastfile_hashes["zombie_pentagon_patch.ff"] = Constants::ZOMBIE_PENTAGON_PATCH;
    fastfile_hashes["zombietron_patch.ff"] = Constants::ZOMBIETRON_PATCH;
    fastfile_hashes["zombie_cosmodrome_patch.ff"] = Constants::ZOMBIE_COSMODROME_PATCH;
    fastfile_hashes["zombie_coast_patch.ff"] = Constants::ZOMBIE_COAST_PATCH;
    fastfile_hashes["zombie_temple_patch.ff"] = Constants::ZOMBIE_TEMPLE_PATCH;
    fastfile_hashes["zombie_moon_patch.ff"] = Constants::ZOMBIE_MOON_PATCH;
    fastfile_hashes["zombie_cod5_prototype_patch.ff"] = Constants::ZOMBIE_COD5_PROTOTYPE_PATCH;
    fastfile_hashes["zombie_cod5_asylum_patch.ff"] = Constants::ZOMBIE_COD5_ASYLUM_PATCH;
    fastfile_hashes["zombie_cod5_sumpf_patch.ff"] = Constants::ZOMBIE_COD5_SUMPF_PATCH;
    fastfile_hashes["zombie_cod5_factory_patch.ff"] = Constants::ZOMBIE_COD5_FACTORY_PATCH;

    mod_fastfile_hashes["mods/Remix"] = Constants::REMIX_MOD;
    mod_fastfile_hashes["mods/FirstRooms"] = Constants::FIRSTROOMS_MOD;
    mod_fastfile_hashes["mods/Song Timing Mod V3"] = Constants::SONG_TIMING_MOD;

    stealth_patch_hashes = {
        "c3ceab590eb62f593a9cbdd59d588243"
    };

    valid_common_files = {
        "code_post_gfx.ff", "code_post_gfx_mp.ff", "code_pre_gfx.ff",
        "code_pre_gfx_mp.ff", "common.ff", "common_mp.ff",
        "common_zombie.ff", "common_zombie_patch.ff", "creek_1.ff",
        "cuba.ff", "flashpoint.ff", "frontend.ff",
        "frontend_patch.ff" /* game_mod related */, "fullahead.ff", "hue_city.ff",
        "int_escape.ff", "khe_sanh.ff", "kowloon.ff",
        "mp_area51.ff", "mp_array.ff", "mp_berlinwall2.ff",
        "mp_cairo.ff", "mp_cosmodrome.ff", "mp_cracked.ff",
        "mp_crisis.ff", "mp_discovery.ff", "mp_drivein.ff",
        "mp_duga.ff", "mp_firingrange.ff", "mp_golfcourse.ff",
        "mp_gridlock.ff", "mp_hanoi.ff", "mp_havoc.ff",
        "mp_hotel.ff", "mp_kowloon.ff", "mp_mountain.ff",
        "mp_nuked.ff", "mp_outskirts.ff", "mp_radiation.ff",
        "mp_russianbase.ff", "mp_silo.ff", "mp_stadium.ff",
        "mp_villa.ff", "mp_zoo.ff", "outro.ff",
        "patch.ff", "patch_mp.ff", "patch_ui_mp.ff",
        "pentagon.ff", "pow.ff", "rebirth.ff",
        "river.ff", "so_narrative1_frontend.ff", "so_narrative2_frontend.ff",
        "so_narrative3_frontend.ff", "so_narrative4_frontend.ff", "so_narrative5_frontend.ff",
        "so_narrative6_frontend.ff", "terminal.ff", "ui_mp.ff",
        "ui_viewer_mp.ff","underwaterbase.ff", "vorkuta.ff",
        "wmd.ff", "wmd_sr71.ff", "zombietron.ff",
        "zombietron_patch.ff", "zombie_coast.ff", "zombie_coast_patch.ff",
        "zombie_cod5_asylum.ff", "zombie_cod5_asylum_patch.ff", "zombie_cod5_factory.ff",
        "zombie_cod5_factory_patch.ff", "zombie_cod5_prototype.ff", "zombie_cod5_prototype_patch.ff",
        "zombie_cod5_sumpf.ff", "zombie_cod5_sumpf_patch.ff", "zombie_cosmodrome.ff",
        "zombie_cosmodrome_patch.ff", "zombie_moon.ff", "zombie_moon_patch.ff",
        "zombie_pentagon.ff", "zombie_pentagon_patch.ff", "zombie_temple.ff",
        "zombie_temple_patch.ff", "zombie_theater.ff", "zombie_theater_patch.ff"
    };
}

// checks a specific hash of a fastfile but only if it has "_patch.ff"
bool GameIntegrity::IsFastfilePatchValid(string map)
{
    GameHandler gh;
    string mapPatch = map + "_patch.ff";
    string zoneCommon = gh.GetZoneCommon() + mapPatch;

    bool modified = false;

    // if it doesnt exist, they probably dont own the map or something, we cant check something that doesnt exist.
    if (!filesystem::exists(zoneCommon))
    {
        return true;
    }

    string hash = GetFileMD5(zoneCommon.c_str());
    return hash == fastfile_hashes.at(mapPatch);
}

// checks for extra files in zone/Common
bool GameIntegrity::DoExtraFilesExist()
{
    GameHandler gh;
    string zone_common = gh.GetZoneCommon();

    if (!gh.IsGameOpen())
    {
        return false;
    }

    for (const auto& entry : filesystem::directory_iterator(zone_common))
    {
        string fileName = entry.path().filename().string();

        if (!filesystem::exists(zone_common + fileName))
        {
            continue;
        }

        if (find(valid_common_files.begin(), valid_common_files.end(), fileName) == valid_common_files.end())
        {
            return true;
        }
    }

    return false;
}

// checks for specific DLL files that should not injected into bo1
// this could range from a stealth patch to other files too
bool GameIntegrity::IsStealthPatchDLLPresent()
{
    Memory mem;
    GameHandler gh;
    HANDLE handle = gh.GetBlackOpsProcess();

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
                    string dllPath = string(modulePath);

                    for (string dllHash : stealth_patch_hashes)
                    {
                        if (GetFileMD5(dllPath) == dllHash)
                        {
                            return true;
                        }
                    }

                    if (modInfo.SizeOfImage == 327680) {
                        return true;
                    }

                    if (dllPath.find("stealth_patch") != string::npos)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// makes sure that the game values like god mode, no target, etc. are as they should be
// i believe for this to properly work in coop, it will need to be run by the host
string GameIntegrity::LookForActiveCheatingBinds()
{
    Memory mem;
    GameHandler gh;

    vector<int> godModeAddresses = { 0x01A79868, 0x01A79BB4, 0x01A79F00, 0x01A7A24C };
    vector<int> noTargetAddresses = { 0x01A79868, 0x01A79BB4, 0x01A79F00, 0x01A7A24C };

    vector<string> binds_found;

    // checks the value of god mode, no target and no clip for all 4 players 
    for (int i = 0; i <= 3; i++)
    {
        int demiGodMode = mem.ReadInt(gh.GetBlackOpsProcess(), godModeAddresses[i]) & 2;
        int godMode = mem.ReadInt(gh.GetBlackOpsProcess(), godModeAddresses[i]) & 1;
        int noTarget = mem.ReadInt(gh.GetBlackOpsProcess(), noTargetAddresses[i]) & 4;

        string player = to_string(i + 1);

        if (demiGodMode == 2)
        {
            binds_found.push_back("Demi God Mode (Player " + player + ")");
        }

        if (godMode == 1)
        {
            binds_found.push_back("God Mode (Player " + player + ")");
        }

        if (noTarget == 4)
        {
            binds_found.push_back("No Target (Player " + player + ")");
        }
    }

    // host only no matter what
    int boxMovablePtr = mem.ReadInt(gh.GetBlackOpsProcess(), 0x026210F4);
    int boxMovableAddress = boxMovablePtr + 0x18;
    int boxMovable = mem.ReadInt(gh.GetBlackOpsProcess(), boxMovableAddress) & 16;

    // check for magic_chest_movable changes
    if (boxMovable == 16)
    {
        binds_found.push_back("Box Moving Disabled");
    }

    string cheats_found;

    for (int i = 0; i < binds_found.size(); i++)
    {
        string bind = binds_found[i];
        cheats_found += bind;
        if (i != binds_found.size() - 1)
        {
            cheats_found += ", ";
        }
    }

    return cheats_found;
}

// checks the current mod file hash, its only really important for community leaderboards
bool GameIntegrity::IsModFileValid()
{
    GameHandler gh;
    string mod_name = gh.GetModName();
    string mod_folder = gh.GetBlackOpsPath() + "/" + mod_name;
    string mod_fastfile = mod_folder + "/mod.ff";
    string actual_md5 = mod_fastfile_hashes[mod_name];
    string local_md5 = GetFileMD5(mod_fastfile);

    if (actual_md5 == "" || !filesystem::exists(mod_fastfile))
    {
        return true;
    }

    return local_md5 == actual_md5;
}

// gets the md5 hash of a file
string GameIntegrity::GetFileMD5(string path)
{
    ifstream inFile(path, ios::binary);

    if (!inFile) {
        return "";
    }

    inFile.seekg(0, ios::end);
    long length = inFile.tellg();
    inFile.seekg(0, ios::beg);

    if (length <= 0) {
        return "";
    }

    vector<char> fileData(length);
    inFile.read(fileData.data(), length);

    if (!inFile) {
        return "";
    }

    string hash = md5(fileData.data(), length);
    return hash;
}