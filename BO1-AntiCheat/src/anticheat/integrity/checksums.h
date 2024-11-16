#include <Windows.h>
#include <string>

using namespace std;

#ifndef Checksums
#define Checksums

// map files
const static string COMMON_ZOMBIE = "d869783a3ea5529769f2abcc624ca19d";
const static string ZOMBIE_THEATER = "d78d8f0dd4e156dd5ca5d923bdf69dfa";
const static string ZOMBIE_PENTAGON = "0c4b5e3bfab719f1ccd7ed5a093cb1b5";
const static string ZOMBIETRON = "3a072109ab7cee152397ad5c5bb7ef5d";
const static string ZOMBIE_COSMODROME = "1f9687fc78414831c8779a3a3796db00";
const static string ZOMBIE_COAST = "6038b2149682580d8d79279ca11aceda";
const static string ZOMBIE_TEMPLE = "e49075f6468a62456e35e927dc8cc3c5";
const static string ZOMBIE_MOON = "75e95dd95c0be1ef0eae2bbd1ea03f8c";
const static string ZOMBIE_COD5_PROTOTYPE = "3579066b4c06d2abe1fb376f57c3f38d";
const static string ZOMBIE_COD5_ASYLUM = "8e8cacd12035dc1042ba505c6ae53fb4";
const static string ZOMBIE_COD5_SUMPF = "5e58490d0fb6032bc93a489f1c5a001a";
const static string ZOMBIE_COD5_FACTORY = "41c15a520ece8735261722e45f2734ad";

// patches
const static string COMMON_ZOMBIE_PATCH = "05cd72982feeaa51c64b6d4fcdb39a6e";
const static string ZOMBIE_THEATER_PATCH = "6539dbae7270814bb501b491c9c0f22c";
const static string ZOMBIE_PENTAGON_PATCH = "c25d171dde11c1cff70e25d60e3a033c";
const static string ZOMBIETRON_PATCH = "ca2cdcdb7daa46bc6a1d7b3181064150";
const static string ZOMBIE_COSMODROME_PATCH = "ca0914df4f73aa6b757191564d28a664";
const static string ZOMBIE_COAST_PATCH = "c99634eeb272ce7e869d58ca2ba8d711";
const static string ZOMBIE_TEMPLE_PATCH = "a18ed6009e15a9110d9f3a7b0c1b2824";
const static string ZOMBIE_MOON_PATCH = "a3fb4e38bdf215d258197bbb1edb7999";
const static string ZOMBIE_COD5_PROTOTYPE_PATCH = "90f5e78f53e8f1a55bfa84adb6fb87cb";
const static string ZOMBIE_COD5_ASYLUM_PATCH = "f7ad43bd7dc7cb35158cf94c64941ac8";
const static string ZOMBIE_COD5_SUMPF_PATCH = "169e3d63b2e8b03f37fc57171720c099";
const static string ZOMBIE_COD5_FACTORY_PATCH = "4cadaa0fb9871347affaf3b222117859";

// zone/English
const static string EN_FRONTEND = "9bd4fcceaef7426703806d6a4c9f95ed";
const static string EN_CODE_POST_GFX = "9dc95d7f03ee5623b7a0591e032f1ff4";
const static string EN_CODE_POST_GFX_MP = "71395179e56d7fcbd8e304c13200a202";
const static string EN_CODE_PRE_GFX = "3143397a471d34fbdec40bda365102bc";
const static string EN_CODE_PRE_GFX_MP = "9acb17d6bc1131b803c4ab3180b7b61c";
const static string EN_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string EN_COMMON_ZOMBIE = "e05868ec089df02829c6573568ec1eaa";
const static string EN_ZOMBIETRON = "483b133b3c31cbd4097a8141f523dcbb";
const static string EN_ZOMBIE_COAST = "22c92df00ce5fccb1d4189d10d7cf3eb";
const static string EN_ZOMBIE_COD5_ASYLUM = "cc9999e7e6846d35a3910d383b9a45f3";
const static string EN_ZOMBIE_COD5_FACTORY = "c71e3a900bf6d59f06c24302e30a40d1";
const static string EN_ZOMBIE_COD5_PROTOTYPE = "de6e71c02864c0e2b03a0efedffd7595";
const static string EN_ZOMBIE_COD5_SUMPF = "296f9a8add391cd83399db270c5c382e";
const static string EN_ZOMBIE_COSMODROME = "7a89c4c1c9ca48ad23e24bd2db3f66b9";
const static string EN_ZOMBIE_MOON = "34d4af68f241ca62b93977eda1cfc55a";
const static string EN_ZOMBIE_PENTAGON = "4affce74187e0d4fb8873992f0aa6c93";
const static string EN_ZOMBIE_TEMPLE = "e4261fe5a80ba604b3b7b596d255a8f9";
const static string EN_ZOMBIE_THEATER = "cf7442a77b7e1604f10151f5ae4a9d22";

// zone/Japanese
const static string JA_FRONTEND = "e5ed54755049ebf401b9ef54e1c23451";
const static string JA_CODE_POST_GFX = "6367c90cb8ceb593fe8f00faf3f1f293";
const static string JA_CODE_POST_GFX_MP = "0a18447c5f7eb807ba0e7fb7deccaed9";
const static string JA_CODE_PRE_GFX = "b01113a259571bd8569a328a896def46";
const static string JA_CODE_PRE_GFX_MP = "c95dd50ffc5e9ad22e8a18b06cbd022a";
const static string JA_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string JA_COMMON_ZOMBIE = "c3e06df1eeb7c45c42b1ec11e7b857b4";
const static string JA_ZOMBIETRON = "5aaa803a4cb2550e8fa67979ca6b7074";
const static string JA_ZOMBIE_COAST = "94bf2b8010f091e01a66a6a258e083dc";
const static string JA_ZOMBIE_COD5_ASYLUM = "b5347def971f40d1db1dbf7605bceefa";
const static string JA_ZOMBIE_COD5_FACTORY = "06832298a2331fc37beecc35756bde58";
const static string JA_ZOMBIE_COD5_PROTOTYPE = "c73110ceabc014304fbe35face74251e";
const static string JA_ZOMBIE_COD5_SUMPF = "4ddeca2d38fd356c9be126af3a490254";
const static string JA_ZOMBIE_COSMODROME = "07eea829149c7b4c7f1440f21c4daf84";
const static string JA_ZOMBIE_MOON = "0393c63a0ae8533db4168a298e73ae60";
const static string JA_ZOMBIE_PENTAGON = "b7da778963bfef067254f6f74c6baf5e";
const static string JA_ZOMBIE_TEMPLE = "8ae6eae522c28339b20c6163ee76308d";
const static string JA_ZOMBIE_THEATER = "0bcc599d4d2cc13f749616ffc32f1650";

// zone/French
const static string FR_FRONTEND = "56f2a937dea24c33246d76b06b3fa8cb";
const static string FR_CODE_POST_GFX = "27768e4955dae3fe92dcb82ece889cf0";
const static string FR_CODE_POST_GFX_MP = "bbff5edf93293efb65439c01ec853d97";
const static string FR_CODE_PRE_GFX = "777a4a0e897917433a1e95ee0a568f26";
const static string FR_CODE_PRE_GFX_MP = "99b9b5159848a67cee1d6ed36d5947ca";
const static string FR_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string FR_COMMON_ZOMBIE = "4d11fe372aad3e73b8d29bd5809ffd48";
const static string FR_ZOMBIETRON = "aeec6560bc8914a46e4da4fbf7f5cc0e";
const static string FR_ZOMBIE_COAST = "e59157026bbc60a6e4a1a1351c00b58d";
const static string FR_ZOMBIE_COD5_ASYLUM = "e066bdb0eae30fabb8de8debd168a63e";
const static string FR_ZOMBIE_COD5_FACTORY = "10d2a4740bf65a661619cca29566e4ca";
const static string FR_ZOMBIE_COD5_PROTOTYPE = "c43c9f4b2743367a50b15a7b92e97d14";
const static string FR_ZOMBIE_COD5_SUMPF = "35879921f4f0f6c7f105f5c4c19cc596";
const static string FR_ZOMBIE_COSMODROME = "e1b29a6a7dd15337638384ae75434d5c";
const static string FR_ZOMBIE_MOON = "dba4c69840bf7056d140a5d6de202f4c";
const static string FR_ZOMBIE_PENTAGON = "686213b016395997d8fda195589a15b2";
const static string FR_ZOMBIE_TEMPLE = "6abff878255c26ec881a6e453d536cc0";
const static string FR_ZOMBIE_THEATER = "516ff597b6ccd860fc938c559c64c83c";

// zone/German
const static string GE_FRONTEND = "56ff671eb52c75be5ece16e8ac4af060";
const static string GE_CODE_POST_GFX = "b4f2fe35053c0e9e152f50b1e6f97751";
const static string GE_CODE_POST_GFX_MP = "39125c996949c48963bc9b1b9bb16a73";
const static string GE_CODE_PRE_GFX = "8849a414a15ed4aa16cf77f748cab244";
const static string GE_CODE_PRE_GFX_MP = "30d8b1b569363f991ae7e43d92f9ae7e";
const static string GE_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string GE_COMMON_ZOMBIE = "9f42c6eb1ec3858eed6b573b98f07106";
const static string GE_ZOMBIETRON = "9a31611d8f12dac42c56f3afc884158a";
const static string GE_ZOMBIE_COAST = "5a7c49f6f06e32e121e712d69ef418bb";
const static string GE_ZOMBIE_COD5_ASYLUM = "580ab6a6e9770d754e87610c1ff69df4";
const static string GE_ZOMBIE_COD5_FACTORY = "65c53264da380fbd0b7d48d5f31b3df7";
const static string GE_ZOMBIE_COD5_PROTOTYPE = "8dc84dcdfc62fcdf957fcd3975f1272f";
const static string GE_ZOMBIE_COD5_SUMPF = "3cbc9130f64660e2f02209a203edf9f9";
const static string GE_ZOMBIE_COSMODROME = "c618c8f3ab1468a4ee039dab4920cb10";
const static string GE_ZOMBIE_MOON = "477fbb9d358edb2df8e64e0cb4e20640";
const static string GE_ZOMBIE_PENTAGON = "289778c6df3331bc62057a85d5596ca5";
const static string GE_ZOMBIE_TEMPLE = "73a9ea5cbdf566b30df026a49a18ab5a";
const static string GE_ZOMBIE_THEATER = "cb76516a289afaf74d7e93839bc26ea0";

// zone/Italian
const static string IT_FRONTEND = "f1c18e014e00ba778dc30a934b9bd2c0";
const static string IT_CODE_POST_GFX = "e439633a69e37a835df7c896cbb91605";
const static string IT_CODE_POST_GFX_MP = "474bcb83f030df952dbbf4495f150639";
const static string IT_CODE_PRE_GFX = "11a176fac82c4f9b7d4bfa196af8bb0b";
const static string IT_CODE_PRE_GFX_MP = "1d3100534580cb8bac353d81d7e4efa5";
const static string IT_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string IT_COMMON_ZOMBIE = "0dacbcc35734308f1edf3a45ca93d4fa";
const static string IT_ZOMBIETRON = "a40e24a1440a01e4c2f5c6627376515e";
const static string IT_ZOMBIE_COAST = "3bf23051c8985fa0143651f11b2ed7bc";
const static string IT_ZOMBIE_COD5_ASYLUM = "a187723ef2ba4caac3f2c6c05b0cb0a5";
const static string IT_ZOMBIE_COD5_FACTORY = "0dc6ef07b147f2906737ee62a7f5c164";
const static string IT_ZOMBIE_COD5_PROTOTYPE = "ab04f862866722c0042834467980aa6b";
const static string IT_ZOMBIE_COD5_SUMPF = "b0650090219179616ca212d56010c000";
const static string IT_ZOMBIE_COSMODROME = "74dd9814340bbb5d0ae907ac1a448874";
const static string IT_ZOMBIE_MOON = "60aadc0a7168926acb436c6edec6df73";
const static string IT_ZOMBIE_PENTAGON = "f2b99691417d134c1f4d2fc60fc1e7e4";
const static string IT_ZOMBIE_TEMPLE = "c4b1293c2284bbfc0643b6ec2661e1d6";
const static string IT_ZOMBIE_THEATER = "bee4fc05315ce0e7466b5ee54a27d0be";

// zone/Spanish
const static string SP_FRONTEND = "fb987d1045b302718c9985a41e056808";
const static string SP_CODE_POST_GFX = "87b32bca1ef14901296bec0d9ec02992";
const static string SP_CODE_POST_GFX_MP = "4493802ecf132ddfa3adb2702b49c2b1";
const static string SP_CODE_PRE_GFX = "61f5898c1dadd9a7b8c39c6dcdc9d0e5";
const static string SP_CODE_PRE_GFX_MP = "f560cf0d28724c9953ceb20c71a04df3";
const static string SP_COMMON = "790a677e9f49a005927acb5092ec9bb9";
const static string SP_COMMON_ZOMBIE = "ab53105c94a8cbcc5029c5a949fb0e91";
const static string SP_ZOMBIETRON = "5cb175f9df55559ec3be1837851a1bf3";
const static string SP_ZOMBIE_COAST = "42d962cfe100577006ea0d3a2dad66c1";
const static string SP_ZOMBIE_COD5_ASYLUM = "b673545e53e8c04789256a1c2fc26ece";
const static string SP_ZOMBIE_COD5_FACTORY = "e68fa58caf2f545231cf8536b540edaa";
const static string SP_ZOMBIE_COD5_PROTOTYPE = "f019f0d6c19b9b2862b4c8275f740a87";
const static string SP_ZOMBIE_COD5_SUMPF = "901bba94c59df7a0426f23240dffb816";
const static string SP_ZOMBIE_COSMODROME = "1ed61a537b4f6a02ffdccf7d85571b75";
const static string SP_ZOMBIE_MOON = "c317fe5ac506f2b04b8e80b58ae9435b";
const static string SP_ZOMBIE_PENTAGON = "00e4c72169a4acd05c6084cb91ebfd34";
const static string SP_ZOMBIE_TEMPLE = "8fd2c5192198550109e19aa1a85e0094";
const static string SP_ZOMBIE_THEATER = "48055acf32bcf311abae2ba9749aabfa";

// game mod files
const static string GAME_MOD_FRONTEND = "06e3b0de38ef784d9bf3fd087187c502";
const static string GAME_MOD_DLL = "015c2fc598947895929ee629ae74bbe2";

// community leaderboard mods
const static string REMIX_MOD = "0eab4b4bb87c07a2bcf5343cc41377ad";
const static string FIRSTROOMS_MOD = "6f1c4a8bccad34e5ec783a5f16a55663";
const static string SONG_TIMING_MOD = "2962c5e032598b5888ae97df18313d76";

#endif