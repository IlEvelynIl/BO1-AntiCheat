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

vector<string> common_fastfiles;
vector<string> english_fastfiles;
vector<string> japanese_fastfiles;
vector<string> french_fastfiles;
vector<string> german_fastfiles;
vector<string> italian_fastfiles;
vector<string> spanish_fastfiles;

vector<string> valid_common_files;
vector<string> valid_english_files;
vector<string> valid_japanese_files;
vector<string> valid_french_files;
vector<string> valid_german_files;
vector<string> valid_italian_files;
vector<string> valid_spanish_files;

map<string, string> fastfile_hashes;
map<string, string> mod_fastfile_hashes;
vector<string> suspicious_modules;

namespace anticheat {
    namespace integrity {

        // initializes everything that we need to check on the game
        void Initialize()
        {
            // these are the fastfiles we will scan every fastfile load
            common_fastfiles.push_back("zone\\Common\\frontend_patch.ff");
            common_fastfiles.push_back("zone\\Common\\common_zombie_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_theater_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_pentagon_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombietron_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_cosmodrome_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_coast_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_temple_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_moon_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_cod5_prototype_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_cod5_asylum_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_cod5_sumpf_patch.ff");
            common_fastfiles.push_back("zone\\Common\\zombie_cod5_factory_patch.ff");

            // zone/English
            english_fastfiles.push_back("zone\\English\\en_frontend.ff");
            english_fastfiles.push_back("zone\\English\\en_code_post_gfx.ff");
            english_fastfiles.push_back("zone\\English\\en_code_post_gfx_mp.ff");
            english_fastfiles.push_back("zone\\English\\en_code_pre_gfx.ff");
            english_fastfiles.push_back("zone\\English\\en_code_pre_gfx_mp.ff");
            english_fastfiles.push_back("zone\\English\\en_common.ff");
            english_fastfiles.push_back("zone\\English\\en_common_zombie.ff");
            english_fastfiles.push_back("zone\\English\\en_zombietron.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_coast.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_cod5_asylum.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_cod5_factory.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_cod5_prototype.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_cod5_sumpf.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_cosmodrome.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_moon.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_pentagon.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_temple.ff");
            english_fastfiles.push_back("zone\\English\\en_zombie_theater.ff");

            // zone/Japanese
            japanese_fastfiles.push_back("zone\\Japanese\\ja_frontend.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_code_post_gfx.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_code_post_gfx_mp.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_code_pre_gfx.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_code_pre_gfx_mp.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_common.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_common_zombie.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombietron.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_coast.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_cod5_asylum.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_cod5_factory.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_cod5_prototype.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_cod5_sumpf.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_cosmodrome.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_moon.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_pentagon.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_temple.ff");
            japanese_fastfiles.push_back("zone\\Japanese\\ja_zombie_theater.ff");

            // zone/French
            french_fastfiles.push_back("zone\\French\\fr_frontend.ff");
            french_fastfiles.push_back("zone\\French\\fr_code_post_gfx.ff");
            french_fastfiles.push_back("zone\\French\\fr_code_post_gfx_mp.ff");
            french_fastfiles.push_back("zone\\French\\fr_code_pre_gfx.ff");
            french_fastfiles.push_back("zone\\French\\fr_code_pre_gfx_mp.ff");
            french_fastfiles.push_back("zone\\French\\fr_common.ff");
            french_fastfiles.push_back("zone\\French\\fr_common_zombie.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombietron.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_coast.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_cod5_asylum.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_cod5_factory.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_cod5_prototype.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_cod5_sumpf.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_cosmodrome.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_moon.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_pentagon.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_temple.ff");
            french_fastfiles.push_back("zone\\French\\fr_zombie_theater.ff");

            // zone/German "austrian"
            german_fastfiles.push_back("zone\\German\\ge_frontend.ff");
            german_fastfiles.push_back("zone\\German\\ge_code_post_gfx.ff");
            german_fastfiles.push_back("zone\\German\\ge_code_post_gfx_mp.ff");
            german_fastfiles.push_back("zone\\German\\ge_code_pre_gfx.ff");
            german_fastfiles.push_back("zone\\German\\ge_code_pre_gfx_mp.ff");
            german_fastfiles.push_back("zone\\German\\ge_common.ff");
            german_fastfiles.push_back("zone\\German\\ge_common_zombie.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombietron.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_coast.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_cod5_asylum.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_cod5_factory.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_cod5_prototype.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_cod5_sumpf.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_cosmodrome.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_moon.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_pentagon.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_temple.ff");
            german_fastfiles.push_back("zone\\German\\ge_zombie_theater.ff");

            // zone/Italian
            italian_fastfiles.push_back("zone\\Italian\\it_frontend.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_code_post_gfx.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_code_post_gfx_mp.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_code_pre_gfx.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_code_pre_gfx_mp.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_common.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_common_zombie.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombietron.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_coast.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_cod5_asylum.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_cod5_factory.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_cod5_prototype.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_cod5_sumpf.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_cosmodrome.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_moon.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_pentagon.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_temple.ff");
            italian_fastfiles.push_back("zone\\Italian\\it_zombie_theater.ff");

            // zone/Spanish
            spanish_fastfiles.push_back("zone\\Spanish\\sp_frontend.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_code_post_gfx.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_code_post_gfx_mp.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_code_pre_gfx.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_code_pre_gfx_mp.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_common.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_common_zombie.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombietron.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_coast.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_cod5_asylum.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_cod5_factory.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_cod5_prototype.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_cod5_sumpf.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_cosmodrome.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_moon.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_pentagon.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_temple.ff");
            spanish_fastfiles.push_back("zone\\Spanish\\sp_zombie_theater.ff");

            // fastfile patch files
            fastfile_hashes.insert(make_pair("zone\\Common\\frontend_patch.ff", Checksums::GAME_MOD_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\Common\\common_zombie_patch.ff", Checksums::COMMON_ZOMBIE_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_theater_patch.ff", Checksums::ZOMBIE_THEATER_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_theater_patch.ff", Checksums::ZOMBIE_THEATER_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_pentagon_patch.ff", Checksums::ZOMBIE_PENTAGON_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombietron_patch.ff", Checksums::ZOMBIETRON_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_cosmodrome_patch.ff", Checksums::ZOMBIE_COSMODROME_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_coast_patch.ff", Checksums::ZOMBIE_COAST_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_temple_patch.ff", Checksums::ZOMBIE_TEMPLE_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_moon_patch.ff", Checksums::ZOMBIE_MOON_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_cod5_prototype_patch.ff", Checksums::ZOMBIE_COD5_PROTOTYPE_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_cod5_asylum_patch.ff", Checksums::ZOMBIE_COD5_ASYLUM_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_cod5_sumpf_patch.ff", Checksums::ZOMBIE_COD5_SUMPF_PATCH));
            fastfile_hashes.insert(make_pair("zone\\Common\\zombie_cod5_factory_patch.ff", Checksums::ZOMBIE_COD5_FACTORY_PATCH));

            // english lang files
            fastfile_hashes.insert(make_pair("zone\\English\\en_frontend.ff", Checksums::EN_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\English\\en_code_post_gfx.ff", Checksums::EN_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\English\\en_code_post_gfx_mp.ff", Checksums::EN_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\English\\en_code_pre_gfx.ff", Checksums::EN_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\English\\en_code_pre_gfx_mp.ff", Checksums::EN_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\English\\en_common.ff", Checksums::EN_COMMON));
            fastfile_hashes.insert(make_pair("zone\\English\\en_common_zombie.ff", Checksums::EN_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombietron.ff", Checksums::EN_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_coast.ff", Checksums::EN_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_cod5_asylum.ff", Checksums::EN_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_cod5_factory.ff", Checksums::EN_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_cod5_prototype.ff", Checksums::EN_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_cod5_sumpf.ff", Checksums::EN_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_cosmodrome.ff", Checksums::EN_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_moon.ff", Checksums::EN_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_pentagon.ff", Checksums::EN_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_temple.ff", Checksums::EN_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\English\\en_zombie_theater.ff", Checksums::EN_ZOMBIE_THEATER));
            
            // japanese lang files
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_frontend.ff", Checksums::JA_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_code_post_gfx.ff", Checksums::JA_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_code_post_gfx_mp.ff", Checksums::JA_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_code_pre_gfx.ff", Checksums::JA_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_code_pre_gfx_mp.ff", Checksums::JA_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_common.ff", Checksums::JA_COMMON));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_common_zombie.ff", Checksums::JA_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombietron.ff", Checksums::JA_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_coast.ff", Checksums::JA_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_cod5_asylum.ff", Checksums::JA_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_cod5_factory.ff", Checksums::JA_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_cod5_prototype.ff", Checksums::JA_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_cod5_sumpf.ff", Checksums::JA_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_cosmodrome.ff", Checksums::JA_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_moon.ff", Checksums::JA_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_pentagon.ff", Checksums::JA_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_temple.ff", Checksums::JA_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\Japanese\\ja_zombie_theater.ff", Checksums::JA_ZOMBIE_THEATER));

            // french lang files
            fastfile_hashes.insert(make_pair("zone\\French\\fr_frontend.ff", Checksums::FR_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_code_post_gfx.ff", Checksums::FR_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_code_post_gfx_mp.ff", Checksums::FR_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_code_pre_gfx.ff", Checksums::FR_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_code_pre_gfx_mp.ff", Checksums::FR_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_common.ff", Checksums::FR_COMMON));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_common_zombie.ff", Checksums::FR_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombietron.ff", Checksums::FR_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_coast.ff", Checksums::FR_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_cod5_asylum.ff", Checksums::FR_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_cod5_factory.ff", Checksums::FR_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_cod5_prototype.ff", Checksums::FR_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_cod5_sumpf.ff", Checksums::FR_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_cosmodrome.ff", Checksums::FR_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_moon.ff", Checksums::FR_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_pentagon.ff", Checksums::FR_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_temple.ff", Checksums::FR_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\French\\fr_zombie_theater.ff", Checksums::FR_ZOMBIE_THEATER));

            // german lang files
            fastfile_hashes.insert(make_pair("zone\\German\\ge_frontend.ff", Checksums::GE_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_code_post_gfx.ff", Checksums::GE_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_code_post_gfx_mp.ff", Checksums::GE_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_code_pre_gfx.ff", Checksums::GE_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_code_pre_gfx_mp.ff", Checksums::GE_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_common.ff", Checksums::GE_COMMON));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_common_zombie.ff", Checksums::GE_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombietron.ff", Checksums::GE_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_coast.ff", Checksums::GE_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_cod5_asylum.ff", Checksums::GE_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_cod5_factory.ff", Checksums::GE_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_cod5_prototype.ff", Checksums::GE_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_cod5_sumpf.ff", Checksums::GE_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_cosmodrome.ff", Checksums::GE_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_moon.ff", Checksums::GE_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_pentagon.ff", Checksums::GE_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_temple.ff", Checksums::GE_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\German\\ge_zombie_theater.ff", Checksums::GE_ZOMBIE_THEATER));

            // italian lang files
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_frontend.ff", Checksums::IT_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_code_post_gfx.ff", Checksums::IT_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_code_post_gfx_mp.ff", Checksums::IT_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_code_pre_gfx.ff", Checksums::IT_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_code_pre_gfx_mp.ff", Checksums::IT_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_common.ff", Checksums::IT_COMMON));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_common_zombie.ff", Checksums::IT_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombietron.ff", Checksums::IT_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_coast.ff", Checksums::IT_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_cod5_asylum.ff", Checksums::IT_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_cod5_factory.ff", Checksums::IT_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_cod5_prototype.ff", Checksums::IT_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_cod5_sumpf.ff", Checksums::IT_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_cosmodrome.ff", Checksums::IT_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_moon.ff", Checksums::IT_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_pentagon.ff", Checksums::IT_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_temple.ff", Checksums::IT_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\Italian\\it_zombie_theater.ff", Checksums::IT_ZOMBIE_THEATER));

            // spanish lang files
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_frontend.ff", Checksums::SP_FRONTEND));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_code_post_gfx.ff", Checksums::SP_CODE_POST_GFX));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_code_post_gfx_mp.ff", Checksums::SP_CODE_POST_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_code_pre_gfx.ff", Checksums::SP_CODE_PRE_GFX));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_code_pre_gfx_mp.ff", Checksums::SP_CODE_PRE_GFX_MP));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_common.ff", Checksums::SP_COMMON));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_common_zombie.ff", Checksums::SP_COMMON_ZOMBIE));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombietron.ff", Checksums::SP_ZOMBIETRON));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_coast.ff", Checksums::SP_ZOMBIE_COAST));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_cod5_asylum.ff", Checksums::SP_ZOMBIE_COD5_ASYLUM));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_cod5_factory.ff", Checksums::SP_ZOMBIE_COD5_FACTORY));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_cod5_prototype.ff", Checksums::SP_ZOMBIE_COD5_PROTOTYPE));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_cod5_sumpf.ff", Checksums::SP_ZOMBIE_COD5_SUMPF));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_cosmodrome.ff", Checksums::SP_ZOMBIE_COSMODROME));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_moon.ff", Checksums::SP_ZOMBIE_MOON));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_pentagon.ff", Checksums::SP_ZOMBIE_PENTAGON));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_temple.ff", Checksums::SP_ZOMBIE_TEMPLE));
            fastfile_hashes.insert(make_pair("zone\\Spanish\\sp_zombie_theater.ff", Checksums::SP_ZOMBIE_THEATER));

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

            // zone lang files
            valid_english_files = {
                "en_code_post_gfx.ff", "en_code_post_gfx_mp.ff", "en_code_pre_gfx.ff",
                "en_code_pre_gfx_mp.ff", "en_common.ff", "en_common_mp.ff",
                "en_common_zombie.ff", "en_creek_1.ff", "en_cuba.ff",
                "en_flashpoint.ff", "en_frontend.ff", "en_fullahead.ff",
                "en_hue_city.ff", "en_int_escape.ff", "en_khe_sanh.ff",
                "en_kowloon.ff", "en_mp_area51.ff", "en_mp_array.ff",
                "en_mp_berlinwall2.ff", "en_mp_cairo.ff", "en_mp_cosmodrome.ff",
                "en_mp_cracked.ff", "en_mp_crisis.ff", "en_mp_discovery.ff",
                "en_mp_drivein.ff", "en_mp_duga.ff", "en_mp_firingrange.ff",
                "en_mp_golfcourse.ff", "en_mp_gridlock.ff", "en_mp_hanoi.ff",
                "en_mp_havoc.ff", "en_mp_hotel.ff", "en_mp_kowloon.ff",
                "en_mp_mountain.ff", "en_mp_nuked.ff", "en_mp_outskirts.ff",
                "en_mp_radiation.ff", "en_mp_russianbase.ff", "en_mp_silo.ff",
                "en_mp_stadium.ff", "en_mp_villa.ff", "en_mp_zoo.ff",
                "en_outro.ff", "en_patch.ff", "en_patch_mp.ff",
                "en_pentagon.ff", "en_pow.ff", "en_rebirth.ff",
                "en_river.ff", "en_so_narrative1_frontend.ff", "en_so_narrative2_frontend.ff",
                "en_so_narrative3_frontend.ff", "en_so_narrative4_frontend.ff", "en_so_narrative5_frontend.ff",
                "en_so_narrative6_frontend.ff", "en_terminal.ff", "en_ui_mp.ff",
                "en_underwaterbase.ff", "en_vorkuta.ff", "en_wmd.ff",
                "en_wmd_sr71.ff", "en_zombietron.ff", "en_zombie_coast.ff",
                "en_zombie_cod5_asylum.ff", "en_zombie_cod5_factory.ff", "en_zombie_cod5_prototype.ff",
                "en_zombie_cod5_sumpf.ff", "en_zombie_cosmodrome.ff", "en_zombie_moon.ff",
                "en_zombie_pentagon.ff", "en_zombie_temple.ff", "en_zombie_theater.ff"
            };

            valid_japanese_files = {
                "ja_code_post_gfx.ff", "ja_code_post_gfx_mp.ff", "ja_code_pre_gfx.ff",
                "ja_code_pre_gfx_mp.ff", "ja_common.ff", "ja_common_mp.ff",
                "ja_common_zombie.ff", "ja_creek_1.ff", "ja_cuba.ff",
                "ja_flashpoint.ff", "ja_frontend.ff", "ja_fullahead.ff",
                "ja_hue_city.ff", "ja_int_escape.ff", "ja_khe_sanh.ff",
                "ja_kowloon.ff", "ja_mp_area51.ff", "ja_mp_array.ff",
                "ja_mp_berlinwall2.ff", "ja_mp_cairo.ff", "ja_mp_cosmodrome.ff",
                "ja_mp_cracked.ff", "ja_mp_crisis.ff", "ja_mp_discovery.ff",
                "ja_mp_drivein.ff", "ja_mp_duga.ff", "ja_mp_firingrange.ff",
                "ja_mp_golfcourse.ff", "ja_mp_gridlock.ff", "ja_mp_hanoi.ff",
                "ja_mp_havoc.ff", "ja_mp_hotel.ff", "ja_mp_kowloon.ff",
                "ja_mp_mountain.ff", "ja_mp_nuked.ff", "ja_mp_outskirts.ff",
                "ja_mp_radiation.ff", "ja_mp_russianbase.ff", "ja_mp_silo.ff",
                "ja_mp_stadium.ff", "ja_mp_villa.ff", "ja_mp_zoo.ff",
                "ja_outro.ff", "ja_patch.ff", "ja_patch_mp.ff",
                "ja_pentagon.ff", "ja_pow.ff", "ja_rebirth.ff",
                "ja_river.ff", "ja_so_narrative1_frontend.ff", "ja_so_narrative2_frontend.ff",
                "ja_so_narrative3_frontend.ff", "ja_so_narrative4_frontend.ff", "ja_so_narrative5_frontend.ff",
                "ja_so_narrative6_frontend.ff", "ja_terminal.ff", "ja_ui_mp.ff",
                "ja_underwaterbase.ff", "ja_vorkuta.ff", "ja_wmd.ff",
                "ja_wmd_sr71.ff", "ja_zombietron.ff", "ja_zombie_coast.ff",
                "ja_zombie_cod5_asylum.ff", "ja_zombie_cod5_factory.ff", "ja_zombie_cod5_prototype.ff",
                "ja_zombie_cod5_sumpf.ff", "ja_zombie_cosmodrome.ff", "ja_zombie_moon.ff",
                "ja_zombie_pentagon.ff", "ja_zombie_temple.ff", "ja_zombie_theater.ff"
            };

            valid_french_files = {
                "fr_code_post_gfx.ff", "fr_code_post_gfx_mp.ff", "fr_code_pre_gfx.ff",
                "fr_code_pre_gfx_mp.ff", "fr_common.ff", "fr_common_mp.ff",
                "fr_common_zombie.ff", "fr_creek_1.ff", "fr_cuba.ff",
                "fr_flashpoint.ff", "fr_frontend.ff", "fr_fullahead.ff",
                "fr_hue_city.ff", "fr_int_escape.ff", "fr_khe_sanh.ff",
                "fr_kowloon.ff", "fr_mp_area51.ff", "fr_mp_array.ff",
                "fr_mp_berlinwall2.ff", "fr_mp_cairo.ff", "fr_mp_cosmodrome.ff",
                "fr_mp_cracked.ff", "fr_mp_crisis.ff", "fr_mp_discovery.ff",
                "fr_mp_drivein.ff", "fr_mp_duga.ff", "fr_mp_firingrange.ff",
                "fr_mp_golfcourse.ff", "fr_mp_gridlock.ff", "fr_mp_hanoi.ff",
                "fr_mp_havoc.ff", "fr_mp_hotel.ff", "fr_mp_kowloon.ff",
                "fr_mp_mountain.ff", "fr_mp_nuked.ff", "fr_mp_outskirts.ff",
                "fr_mp_radiation.ff", "fr_mp_russianbase.ff", "fr_mp_silo.ff",
                "fr_mp_stadium.ff", "fr_mp_villa.ff", "fr_mp_zoo.ff",
                "fr_outro.ff", "fr_patch.ff", "fr_patch_mp.ff",
                "fr_pentagon.ff", "fr_pow.ff", "fr_rebirth.ff",
                "fr_river.ff", "fr_so_narrative1_frontend.ff", "fr_so_narrative2_frontend.ff",
                "fr_so_narrative3_frontend.ff", "fr_so_narrative4_frontend.ff", "fr_so_narrative5_frontend.ff",
                "fr_so_narrative6_frontend.ff", "fr_terminal.ff", "fr_ui_mp.ff",
                "fr_underwaterbase.ff", "fr_vorkuta.ff", "fr_wmd.ff",
                "fr_wmd_sr71.ff", "fr_zombietron.ff", "fr_zombie_coast.ff",
                "fr_zombie_cod5_asylum.ff", "fr_zombie_cod5_factory.ff", "fr_zombie_cod5_prototype.ff",
                "fr_zombie_cod5_sumpf.ff", "fr_zombie_cosmodrome.ff", "fr_zombie_moon.ff",
                "fr_zombie_pentagon.ff", "fr_zombie_temple.ff", "fr_zombie_theater.ff"
            };

            valid_german_files = {
                "ge_code_post_gfx.ff", "ge_code_post_gfx_mp.ff", "ge_code_pre_gfx.ff",
                "ge_code_pre_gfx_mp.ff", "ge_common.ff", "ge_common_mp.ff",
                "ge_common_zombie.ff", "ge_creek_1.ff", "ge_cuba.ff",
                "ge_flashpoint.ff", "ge_frontend.ff", "ge_fullahead.ff",
                "ge_hue_city.ff", "ge_int_escape.ff", "ge_khe_sanh.ff",
                "ge_kowloon.ff", "ge_mp_area51.ff", "ge_mp_array.ff",
                "ge_mp_berlinwall2.ff", "ge_mp_cairo.ff", "ge_mp_cosmodrome.ff",
                "ge_mp_cracked.ff", "ge_mp_crisis.ff", "ge_mp_discovery.ff",
                "ge_mp_drivein.ff", "ge_mp_duga.ff", "ge_mp_firingrange.ff",
                "ge_mp_golfcourse.ff", "ge_mp_gridlock.ff", "ge_mp_hanoi.ff",
                "ge_mp_havoc.ff", "ge_mp_hotel.ff", "ge_mp_kowloon.ff",
                "ge_mp_mountain.ff", "ge_mp_nuked.ff", "ge_mp_outskirts.ff",
                "ge_mp_radiation.ff", "ge_mp_russianbase.ff", "ge_mp_silo.ff",
                "ge_mp_stadium.ff", "ge_mp_villa.ff", "ge_mp_zoo.ff",
                "ge_outro.ff", "ge_patch.ff", "ge_patch_mp.ff",
                "ge_pentagon.ff", "ge_pow.ff", "ge_rebirth.ff",
                "ge_river.ff", "ge_so_narrative1_frontend.ff", "ge_so_narrative2_frontend.ff",
                "ge_so_narrative3_frontend.ff", "ge_so_narrative4_frontend.ff", "ge_so_narrative5_frontend.ff",
                "ge_so_narrative6_frontend.ff", "ge_terminal.ff", "ge_ui_mp.ff",
                "ge_underwaterbase.ff", "ge_vorkuta.ff", "ge_wmd.ff",
                "ge_wmd_sr71.ff", "ge_zombietron.ff", "ge_zombie_coast.ff",
                "ge_zombie_cod5_asylum.ff", "ge_zombie_cod5_factory.ff", "ge_zombie_cod5_prototype.ff",
                "ge_zombie_cod5_sumpf.ff", "ge_zombie_cosmodrome.ff", "ge_zombie_moon.ff",
                "ge_zombie_pentagon.ff", "ge_zombie_temple.ff", "ge_zombie_theater.ff"
            };

            valid_italian_files = {
                "it_code_post_gfx.ff", "it_code_post_gfx_mp.ff", "it_code_pre_gfx.ff",
                "it_code_pre_gfx_mp.ff", "it_common.ff", "it_common_mp.ff",
                "it_common_zombie.ff", "it_creek_1.ff", "it_cuba.ff",
                "it_flashpoint.ff", "it_frontend.ff", "it_fullahead.ff",
                "it_hue_city.ff", "it_int_escape.ff", "it_khe_sanh.ff",
                "it_kowloon.ff", "it_mp_area51.ff", "it_mp_array.ff",
                "it_mp_berlinwall2.ff", "it_mp_cairo.ff", "it_mp_cosmodrome.ff",
                "it_mp_cracked.ff", "it_mp_crisis.ff", "it_mp_discovery.ff",
                "it_mp_drivein.ff", "it_mp_duga.ff", "it_mp_firingrange.ff",
                "it_mp_golfcourse.ff", "it_mp_gridlock.ff", "it_mp_hanoi.ff",
                "it_mp_havoc.ff", "it_mp_hotel.ff", "it_mp_kowloon.ff",
                "it_mp_mountain.ff", "it_mp_nuked.ff", "it_mp_outskirts.ff",
                "it_mp_radiation.ff", "it_mp_russianbase.ff", "it_mp_silo.ff",
                "it_mp_stadium.ff", "it_mp_villa.ff", "it_mp_zoo.ff",
                "it_outro.ff", "it_patch.ff", "it_patch_mp.ff",
                "it_pentagon.ff", "it_pow.ff", "it_rebirth.ff",
                "it_river.ff", "it_so_narrative1_frontend.ff", "it_so_narrative2_frontend.ff",
                "it_so_narrative3_frontend.ff", "it_so_narrative4_frontend.ff", "it_so_narrative5_frontend.ff",
                "it_so_narrative6_frontend.ff", "it_terminal.ff", "it_ui_mp.ff",
                "it_underwaterbase.ff", "it_vorkuta.ff", "it_wmd.ff",
                "it_wmd_sr71.ff", "it_zombietron.ff", "it_zombie_coast.ff",
                "it_zombie_cod5_asylum.ff", "it_zombie_cod5_factory.ff", "it_zombie_cod5_prototype.ff",
                "it_zombie_cod5_sumpf.ff", "it_zombie_cosmodrome.ff", "it_zombie_moon.ff",
                "it_zombie_pentagon.ff", "it_zombie_temple.ff", "it_zombie_theater.ff"
            };

            valid_spanish_files = {
                "sp_code_post_gfx.ff", "sp_code_post_gfx_mp.ff", "sp_code_pre_gfx.ff",
                "sp_code_pre_gfx_mp.ff", "sp_common.ff", "sp_common_mp.ff",
                "sp_common_zombie.ff", "sp_creek_1.ff", "sp_cuba.ff",
                "sp_flashpoint.ff", "sp_frontend.ff", "sp_fullahead.ff",
                "sp_hue_city.ff", "sp_int_escape.ff", "sp_khe_sanh.ff",
                "sp_kowloon.ff", "sp_mp_area51.ff", "sp_mp_array.ff",
                "sp_mp_berlinwall2.ff", "sp_mp_cairo.ff", "sp_mp_cosmodrome.ff",
                "sp_mp_cracked.ff", "sp_mp_crisis.ff", "sp_mp_discovery.ff",
                "sp_mp_drivein.ff", "sp_mp_duga.ff", "sp_mp_firingrange.ff",
                "sp_mp_golfcourse.ff", "sp_mp_gridlock.ff", "sp_mp_hanoi.ff",
                "sp_mp_havoc.ff", "sp_mp_hotel.ff", "sp_mp_kowloon.ff",
                "sp_mp_mountain.ff", "sp_mp_nuked.ff", "sp_mp_outskirts.ff",
                "sp_mp_radiation.ff", "sp_mp_russianbase.ff", "sp_mp_silo.ff",
                "sp_mp_stadium.ff", "sp_mp_villa.ff", "sp_mp_zoo.ff",
                "sp_outro.ff", "sp_patch.ff", "sp_patch_mp.ff",
                "sp_pentagon.ff", "sp_pow.ff", "sp_rebirth.ff",
                "sp_river.ff", "sp_so_narrative1_frontend.ff", "sp_so_narrative2_frontend.ff",
                "sp_so_narrative3_frontend.ff", "sp_so_narrative4_frontend.ff", "sp_so_narrative5_frontend.ff",
                "sp_so_narrative6_frontend.ff", "sp_terminal.ff", "sp_ui_mp.ff",
                "sp_underwaterbase.ff", "sp_vorkuta.ff", "sp_wmd.ff",
                "sp_wmd_sr71.ff", "sp_zombietron.ff", "sp_zombie_coast.ff",
                "sp_zombie_cod5_asylum.ff", "sp_zombie_cod5_factory.ff", "sp_zombie_cod5_prototype.ff",
                "sp_zombie_cod5_sumpf.ff", "sp_zombie_cosmodrome.ff", "sp_zombie_moon.ff",
                "sp_zombie_pentagon.ff", "sp_zombie_temple.ff", "sp_zombie_theater.ff"
            };
        }

        // checks a specific hash of a fastfile but only if it has "_patch.ff"
        bool IsFastfileValid(string fastfile)
        {
            string zoneFile = game::GetBlackOpsPath() + "\\" + fastfile;

            // if it doesnt exist, they probably dont own the map or something, we cant check something that doesnt exist.
            if (!filesystem::exists(zoneFile))
            {
                return true;
            }

            string hash = GetFileMD5(zoneFile);
            return hash == fastfile_hashes.at(fastfile);
        }

        std::string GetModifiedFastfiles()
        {
            vector<string> modified_fastfiles;

            // check every single fastfile patch
            for (string fastfile : common_fastfiles)
            {
                if (!IsFastfileValid(fastfile))
                {
                    modified_fastfiles.push_back(fastfile);
                }
            }

            // format all fastfiles that were modified into a nice list
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

        std::string GetModifiedLangFiles()
        {
            string lang = game::GetGameLanguage();
            vector<string> lang_files;

            if (lang == "english")
            {
                lang_files = english_fastfiles;
            }
            else if (lang == "japanese")
            {
                lang_files = japanese_fastfiles;
            }
            else if (lang == "french")
            {
                lang_files = french_fastfiles;
            }
            else if (lang == "austrian")
            {
                lang_files = german_fastfiles;
            }
            else if (lang == "italian")
            {
                lang_files = italian_fastfiles;
            }
            else if (lang == "spanish")
            {
                lang_files = spanish_fastfiles;
            }

            vector<string> modified_fastfiles;

            // check every single lang fastfile
            for (string fastfile : lang_files)
            {
                if (!IsFastfileValid(fastfile))
                {
                    modified_fastfiles.push_back(fastfile);
                }
            }

            // format all fastfiles that were modified into a nice list
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

        // checks for extra files in zone/Common & the language folder
        bool DoExtraFilesExist()
        {
            string zone_common = game::GetZoneCommon();
            string zone_language = game::GetZoneLanguage();

            if (!game::process::IsGameOpen())
            {
                return false;
            }

            // check zone/Common for any different files
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

            // if for some reason they dont have the language installed
            if (!filesystem::exists(zone_language))
            {
                return false;
            }

            // get the proper list of files to check against
            vector<string> lang_folder_files;
            string lang = game::GetGameLanguage();

            if (lang == "english")
            {
                lang_folder_files = valid_english_files;
            }
            else if (lang == "japanese")
            {
                lang_folder_files = valid_japanese_files;
            }
            else if (lang == "french")
            {
                lang_folder_files = valid_french_files;
            }
            else if (lang == "austrian")
            {
                lang_folder_files = valid_german_files;
            }
            else if (lang == "italian")
            {
                lang_folder_files = valid_italian_files;
            }
            else if (lang == "spanish")
            {
                lang_folder_files = valid_spanish_files;
            }

            // check the current language folder
            for (const auto& entry : filesystem::directory_iterator(zone_language))
            {
                string fileName = entry.path().filename().string();

                if (!filesystem::exists(zone_language + fileName))
                {
                    continue;
                }

                if (find(lang_folder_files.begin(), lang_folder_files.end(), fileName) == lang_folder_files.end())
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