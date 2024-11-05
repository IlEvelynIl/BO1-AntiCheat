#include "integrity.hpp"

#include "../../game/game.hpp"

#include "../../game/process.hpp"

#include "checksums.h"

#include "../../constants.h"

#include "../../utils/md5.h"

#include "../../utils/memory.hpp"

#include <fstream>

#include <sstream>

using namespace std;

vector<string> fastfile_queue;
map<string, string> fastfile_hashes;
map<string, string> mod_fastfile_hashes;
vector<string> valid_common_files;
vector<string> suspicious_modules;

namespace anticheat {
    namespace integrity {

        // initializes everything that we need to check on the game
        void Initialize()
        {
            // these are the fastfiles we will scan every map load
            fastfile_queue.push_back("frontend_patch.ff");
            fastfile_queue.push_back("common_zombie_patch.ff");
            fastfile_queue.push_back("zombie_theater_patch.ff");
            fastfile_queue.push_back("zombie_pentagon_patch.ff");
            fastfile_queue.push_back("zombietron_patch.ff");
            fastfile_queue.push_back("zombie_cosmodrome_patch.ff");
            fastfile_queue.push_back("zombie_coast_patch.ff");
            fastfile_queue.push_back("zombie_temple_patch.ff");
            fastfile_queue.push_back("zombie_moon_patch.ff");
            fastfile_queue.push_back("zombie_cod5_prototype_patch.ff");
            fastfile_queue.push_back("zombie_cod5_asylum_patch.ff");
            fastfile_queue.push_back("zombie_cod5_sumpf_patch.ff");
            fastfile_queue.push_back("zombie_cod5_factory_patch.ff");

            // fastfile patch files
            fastfile_hashes.insert(make_pair("frontend_patch.ff", Checksums::GAME_MOD_FRONTEND));
            fastfile_hashes.insert(make_pair("common_zombie_patch.ff", Checksums::COMMON_ZOMBIE_PATCH));
            fastfile_hashes.insert(make_pair("zombie_theater_patch.ff", Checksums::ZOMBIE_THEATER_PATCH));
            fastfile_hashes.insert(make_pair("zombie_theater_patch.ff", Checksums::ZOMBIE_THEATER_PATCH));
            fastfile_hashes.insert(make_pair("zombie_pentagon_patch.ff", Checksums::ZOMBIE_PENTAGON_PATCH));
            fastfile_hashes.insert(make_pair("zombietron_patch.ff", Checksums::ZOMBIETRON_PATCH));
            fastfile_hashes.insert(make_pair("zombie_cosmodrome_patch.ff", Checksums::ZOMBIE_COSMODROME_PATCH));
            fastfile_hashes.insert(make_pair("zombie_coast_patch.ff", Checksums::ZOMBIE_COAST_PATCH));
            fastfile_hashes.insert(make_pair("zombie_temple_patch.ff", Checksums::ZOMBIE_TEMPLE_PATCH));
            fastfile_hashes.insert(make_pair("zombie_moon_patch.ff", Checksums::ZOMBIE_MOON_PATCH));
            fastfile_hashes.insert(make_pair("zombie_cod5_prototype_patch.ff", Checksums::ZOMBIE_COD5_PROTOTYPE_PATCH));
            fastfile_hashes.insert(make_pair("zombie_cod5_asylum_patch.ff", Checksums::ZOMBIE_COD5_ASYLUM_PATCH));
            fastfile_hashes.insert(make_pair("zombie_cod5_sumpf_patch.ff", Checksums::ZOMBIE_COD5_SUMPF_PATCH));
            fastfile_hashes.insert(make_pair("zombie_cod5_factory_patch.ff", Checksums::ZOMBIE_COD5_FACTORY_PATCH));

            // community leaderboard mod files
            mod_fastfile_hashes.insert(make_pair("mods/Remix", Checksums::REMIX_MOD));
            mod_fastfile_hashes.insert(make_pair("mods/FirstRooms", Checksums::FIRSTROOMS_MOD));
            mod_fastfile_hashes.insert(make_pair("mods/Song Timing Mod V3", Checksums::SONG_TIMING_MOD));

            // any known modules that can be injected into the game for cheating purposes
            suspicious_modules = {
                "c3ceab590eb62f593a9cbdd59d588243" // stealth_patch.dll
            };

            // all files that should be in the zone/Common folder
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
        bool IsFastfileValid(string map)
        {
            string zoneCommon = game::GetZoneCommon() + map;

            // if it doesnt exist, they probably dont own the map or something, we cant check something that doesnt exist.
            if (!filesystem::exists(zoneCommon))
            {
                return true;
            }

            string hash = GetFileMD5(zoneCommon);
            return hash == fastfile_hashes.at(map);
        }

        std::string GetModifiedFastfiles()
        {
            vector<string> modified_fastfiles;

            // check every single fastfile patches
            for (string fastfile : fastfile_queue)
            {
                if (!IsFastfileValid(fastfile))
                {
                    modified_fastfiles.push_back(fastfile);
                }
            }

            // format all cheating binds detected into a nice list
            string formatted_fastfiles = "";
            for (int i = 0; i < modified_fastfiles.size(); i++)
            {
                string fastfile = modified_fastfiles[i];
                formatted_fastfiles += fastfile;
                if (i != modified_fastfiles.size() - 1)
                {
                    formatted_fastfiles += ", ";
                }
            }

            return formatted_fastfiles;
        }

        // checks for extra files in zone/Common
        bool DoExtraFilesExist()
        {
            string zone_common = game::GetZoneCommon();

            if (!game::process::IsGameOpen())
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
        bool IsStealthPatchInjected()
        {
            if (!game::process::IsGameOpen())
            {
                return false;
            }

            HANDLE handle = game::process::GetBlackOpsProcess();
            std::vector<HMODULE> hMods(1024);
            DWORD cbNeeded;

            if (EnumProcessModulesEx(handle, hMods.data(), hMods.size() * sizeof(HMODULE), &cbNeeded, LIST_MODULES_ALL)) {
                size_t moduleCount = cbNeeded / sizeof(HMODULE);
                for (size_t i = 0; i < moduleCount; i++) {
                    if (IsSuspiciousModule(hMods[i], handle)) {
                        return true;
                    }
                }
            }

            return false;
        }

        // checks the current mod file hash, its only really important for community leaderboards
        bool IsModFileValid()
        {
            string mod_name = game::GetModName();
            string mod_folder = game::GetBlackOpsPath() + "/" + mod_name;
            string mod_fastfile = mod_folder + "/mod.ff";
            string actual_md5 = mod_fastfile_hashes[mod_name];
            string local_md5 = GetFileMD5(mod_fastfile.c_str());

            if (actual_md5 == "" || !filesystem::exists(mod_fastfile))
            {
                return true;
            }

            return local_md5 == actual_md5;
        }

        // makes sure a module isnt something like a stealth patch dll or something
        bool IsSuspiciousModule(HMODULE hModule, HANDLE hProcess) {
            TCHAR szModName[MAX_PATH];
            MODULEINFO modInfo;

            // Get module file name
            if (GetModuleFileNameEx(hProcess, hModule, szModName, sizeof(szModName) / sizeof(TCHAR))) {
                if (GetModuleInformation(hProcess, hModule, &modInfo, sizeof(modInfo))) {
                    char modulePath[MAX_PATH];
                    size_t convertedChars = 0;
                    wcstombs_s(&convertedChars, modulePath, szModName, sizeof(modulePath));
                    modulePath[sizeof(modulePath) - 1] = '\0';
                    std::string dllPath = std::string(modulePath);

                    // Check against suspicious_modules
                    for (const std::string& dllHash : suspicious_modules) {
                        if (GetFileMD5(dllPath) == dllHash) {
                            return true; // MD5 match found
                        }
                    }

                    // Check for specific image size of the original stealth_patch.dll file
                    if (modInfo.SizeOfImage == 327680) {
                        return true; // Suspected image size
                    }

                    // Check for suspicious module names
                    if (dllPath.find("stealth_patch") != std::string::npos) {
                        return true; // Module path contains 'stealth_patch'
                    }
                }
            }

            return false; // Not suspicious
        }

        // makes sure that the game values like god mode, no target, etc. are not modified
        // i believe for this to properly work in coop, it will need to be run by the host
        std::string GetActiveCheatingBinds()
        {
            vector<int> godModeAddresses = { 0x01A79868, 0x01A79BB4, 0x01A79F00, 0x01A7A24C };
            vector<int> noTargetAddresses = { 0x01A79868, 0x01A79BB4, 0x01A79F00, 0x01A7A24C };

            vector<string> binds_found;

            // checks the player states for all 4
            for (int i = 0; i <= 3; i++)
            {
                int demiGodMode = memory::ReadInt(game::process::GetBlackOpsProcess(), godModeAddresses[i]) & 2;
                int godMode = memory::ReadInt(game::process::GetBlackOpsProcess(), godModeAddresses[i]) & 1;
                int noTarget = memory::ReadInt(game::process::GetBlackOpsProcess(), noTargetAddresses[i]) & 4;

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

            // check for magic_chest_movable changes
            int boxMovablePtr = memory::ReadInt(game::process::GetBlackOpsProcess(), 0x026210F4);
            int boxMovableAddress = boxMovablePtr + 0x18;
            int boxMovable = memory::ReadInt(game::process::GetBlackOpsProcess(), boxMovableAddress)/* & 16*/;
            int boxMovableDisabled = game::IsGameModLoaded() ? 50759780 : 50759732;

            if (boxMovable == boxMovableDisabled)
            {
                binds_found.push_back("Box Moving Disabled");
            }

            // format all cheating binds detected into a nice list
            string formatted_binds_found;
            for (int i = 0; i < binds_found.size(); i++)
            {
                string bind = binds_found[i];
                formatted_binds_found += bind;
                if (i != binds_found.size() - 1)
                {
                    formatted_binds_found += ", ";
                }
            }

            return formatted_binds_found;
        }

        // gets the md5 hash of a file
        std::string GetFileMD5(string path)
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
            return hash.c_str();
        }
    } // integrity
} // anticheat