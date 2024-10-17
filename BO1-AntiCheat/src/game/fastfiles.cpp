#include "../game/fastfiles.hpp"

#include "../game/game_handler.hpp"

#include "../constants.h"

#include "../utilities/md5.h"

#include "../display/display.hpp"

#include "../display/statuses.h"

#include <fstream>

#include <unordered_set>

using namespace std;

map<string, string> fastFileHashes;
vector<string> validFiles;

void FastFiles::init()
{
    // base scannable_maps
    fastFileHashes["zombie_theater.ff"] = Constants::ZOMBIE_THEATER;
    fastFileHashes["zombie_theater_patch.ff"] = Constants::ZOMBIE_THEATER_PATCH;

    fastFileHashes["zombie_pentagon.ff"] = Constants::ZOMBIE_PENTAGON;
    fastFileHashes["zombie_pentagon_patch.ff"] = Constants::ZOMBIE_PENTAGON_PATCH;

    // dlc
    fastFileHashes["zombie_cosmodrome.ff"] = Constants::ZOMBIE_COSMODROME;
    fastFileHashes["zombie_cosmodrome_patch.ff"] = Constants::ZOMBIE_COSMODROME_PATCH;

    fastFileHashes["zombie_coast.ff"] = Constants::ZOMBIE_COAST;
    fastFileHashes["zombie_coast_patch.ff"] = Constants::ZOMBIE_COAST_PATCH;

    fastFileHashes["zombie_temple.ff"] = Constants::ZOMBIE_TEMPLE;
    fastFileHashes["zombie_temple_patch.ff"] = Constants::ZOMBIE_TEMPLE_PATCH;

    fastFileHashes["zombie_moon.ff"] = Constants::ZOMBIE_MOON;
    fastFileHashes["zombie_moon_patch.ff"] = Constants::ZOMBIE_MOON_PATCH;

    // waw remasters
    fastFileHashes["zombie_cod5_prototype.ff"] = Constants::ZOMBIE_COD5_PROTOTYPE;
    fastFileHashes["zombie_cod5_prototype_patch.ff"] = Constants::ZOMBIE_COD5_PROTOTYPE_PATCH;

    fastFileHashes["zombie_cod5_asylum.ff"] = Constants::ZOMBIE_COD5_ASYLUM;
    fastFileHashes["zombie_cod5_asylum_patch.ff"] = Constants::ZOMBIE_COD5_ASYLUM_PATCH;

    fastFileHashes["zombie_cod5_sumpf.ff"] = Constants::ZOMBIE_COD5_SUMPF;
    fastFileHashes["zombie_cod5_sumpf_patch.ff"] = Constants::ZOMBIE_COD5_SUMPF_PATCH;

    fastFileHashes["zombie_cod5_factory.ff"] = Constants::ZOMBIE_COD5_FACTORY;
    fastFileHashes["zombie_cod5_factory_patch.ff"] = Constants::ZOMBIE_COD5_FACTORY_PATCH;

    // 86 files
    std::vector<std::string> filesToAdd = {
        "code_post_gfx.ff",
        "code_post_gfx_mp.ff",
        "code_pre_gfx.ff",
        "code_pre_gfx_mp.ff",
        "common.ff",
        "common_mp.ff",
        "common_zombie.ff",
        "common_zombie_patch.ff",
        "creek_1.ff",
        "cuba.ff",
        "flashpoint.ff",
        "frontend.ff",
        "frontend_patch.ff",
        "fullahead.ff",
        "hue_city.ff",
        "int_escape.ff",
        "khe_sanh.ff",
        "kowloon.ff",
        "mp_area51.ff",
        "mp_array.ff",
        "mp_berlinwall2.ff",
        "mp_cairo.ff",
        "mp_cosmodrome.ff",
        "mp_cracked.ff",
        "mp_crisis.ff",
        "mp_discovery.ff",
        "mp_drivein.ff",
        "mp_duga.ff",
        "mp_firingrange.ff",
        "mp_golfcourse.ff",
        "mp_gridlock.ff",
        "mp_hanoi.ff",
        "mp_havoc.ff",
        "mp_hotel.ff",
        "mp_kowloon.ff",
        "mp_mountain.ff",
        "mp_nuked.ff",
        "mp_outskirts.ff",
        "mp_radiation.ff",
        "mp_russianbase.ff",
        "mp_silo.ff",
        "mp_stadium.ff",
        "mp_villa.ff",
        "mp_zoo.ff",
        "outro.ff",
        "patch.ff",
        "patch_mp.ff",
        "patch_ui_mp.ff",
        "pentagon.ff",
        "pow.ff",
        "rebirth.ff",
        "river.ff",
        "so_narrative1_frontend.ff",
        "so_narrative2_frontend.ff",
        "so_narrative3_frontend.ff",
        "so_narrative4_frontend.ff",
        "so_narrative5_frontend.ff",
        "so_narrative6_frontend.ff",
        "terminal.ff",
        "ui_mp.ff",
        "ui_viewer_mp.ff",
        "underwaterbase.ff",
        "vorkuta.ff",
        "wmd.ff",
        "wmd_sr71.ff",
        "zombietron.ff",
        "zombietron_patch.ff",
        "zombie_coast.ff",
        "zombie_coast_patch.ff",
        "zombie_cod5_asylum.ff",
        "zombie_cod5_asylum_patch.ff",
        "zombie_cod5_factory.ff",
        "zombie_cod5_factory_patch.ff",
        "zombie_cod5_prototype.ff",
        "zombie_cod5_prototype_patch.ff",
        "zombie_cod5_sumpf.ff",
        "zombie_cod5_sumpf_patch.ff",
        "zombie_cosmodrome.ff",
        "zombie_cosmodrome_patch.ff",
        "zombie_moon.ff",
        "zombie_moon_patch.ff",
        "zombie_pentagon.ff",
        "zombie_pentagon_patch.ff",
        "zombie_temple.ff",
        "zombie_temple_patch.ff",
        "zombie_theater.ff",
        "zombie_theater_patch.ff"
    };

    for (const auto& file : filesToAdd) {
        validFiles.push_back(file);
    }
}

bool FastFiles::MapFastFileValid(string map)
{
    GameHandler gh;
    bool modded = false;

    string mapPatch = map + "_patch.ff";
    string zoneCommon = gh.GetZoneCommon() + mapPatch;

    bool modified = false;

    if (!std::filesystem::exists(zoneCommon))
    {
        return true;
    }

    string hash = GetPatchMD5(zoneCommon.c_str());
    return hash == fastFileHashes.at(mapPatch);
}

bool FastFiles::CommonZombiePatchValid()
{
    GameHandler gh;
    string patch = gh.GetZoneCommon() + "common_zombie_patch.ff";

    if (!std::filesystem::exists(patch))
    {
        return true;
    }

    string hash = GetPatchMD5(patch.c_str());
    return hash == Constants::COMMON_ZOMBIE_PATCH;
}

bool FastFiles::Game_ModFrontendPatchValid()
{
    GameHandler gh;
    string frontend_patch = gh.GetZoneCommon() + "frontend_patch.ff";

    if (!std::filesystem::exists(frontend_patch))
    {
        return true;
    }

    string hash = GetPatchMD5(frontend_patch.c_str());
    return hash == Constants::GAME_MOD_FRONTEND;
}

bool FastFiles::ExtraFilesExist()
{
    GameHandler gh;
    string zoneCommon = gh.GetZoneCommon();

    if (!gh.IsGameOpen())
    {
        return false;
    }

    for (const auto& entry : std::filesystem::directory_iterator(zoneCommon))
    {
        string fileName = entry.path().filename().string();

        if (!std::filesystem::exists(zoneCommon + fileName))
        {
            continue;
        }

        if (std::find(validFiles.begin(), validFiles.end(), fileName) == validFiles.end())
        {
            return true;
        }
    }

    return false;
}


string FastFiles::GetPatchMD5(string path)
{
    ifstream inBigArrayfile;
    inBigArrayfile.open(path, std::ios::binary | std::ios::in);
    
    
    inBigArrayfile.seekg(0, std::ios::end);
    long length = inBigArrayfile.tellg();
    inBigArrayfile.seekg(0, std::ios::beg);

    char* fileData = new char[length];
    inBigArrayfile.read(fileData, length);

    std::string hash = md5(fileData, length);

    delete[] fileData;

    return hash.c_str();
}