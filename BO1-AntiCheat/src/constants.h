#include <iostream>
#include <windows.h>
#include <string>

using std::string;

#ifndef Constants
#define Constants

const static string VERSION = "1.1";
const static LPCWSTR TITLE = L"BO1 Anti Cheat v1.1";

const static int C_MAPADDRESS = 0x02F67B6C;
const static int C_MODADDRESS = 0x025FE010;

const static int MAIN_MENU_ID = 5;

const static string COMMON_ZOMBIE = "d869783a3ea5529769f2abcc624ca19d";
const static string COMMON_ZOMBIE_PATCH = "05cd72982feeaa51c64b6d4fcdb39a6e";

const static string ZOMBIE_THEATER = "d78d8f0dd4e156dd5ca5d923bdf69dfa";
const static string ZOMBIE_THEATER_PATCH = "6539dbae7270814bb501b491c9c0f22c";

const static string ZOMBIE_PENTAGON = "0c4b5e3bfab719f1ccd7ed5a093cb1b5";
const static string ZOMBIE_PENTAGON_PATCH = "c25d171dde11c1cff70e25d60e3a033c";

const static string ZOMBIETRON = "3a072109ab7cee152397ad5c5bb7ef5d";
const static string ZOMBIETRON_PATCH = "ca2cdcdb7daa46bc6a1d7b3181064150";

const static string ZOMBIE_COSMODROME = "1f9687fc78414831c8779a3a3796db00";
const static string ZOMBIE_COSMODROME_PATCH = "ca0914df4f73aa6b757191564d28a664";

const static string ZOMBIE_COAST = "6038b2149682580d8d79279ca11aceda";
const static string ZOMBIE_COAST_PATCH = "c99634eeb272ce7e869d58ca2ba8d711";

const static string ZOMBIE_TEMPLE = "e49075f6468a62456e35e927dc8cc3c5";
const static string ZOMBIE_TEMPLE_PATCH = "a18ed6009e15a9110d9f3a7b0c1b2824";

const static string ZOMBIE_MOON = "75e95dd95c0be1ef0eae2bbd1ea03f8c";
const static string ZOMBIE_MOON_PATCH = "a3fb4e38bdf215d258197bbb1edb7999";

const static string ZOMBIE_COD5_PROTOTYPE = "3579066b4c06d2abe1fb376f57c3f38d";
const static string ZOMBIE_COD5_PROTOTYPE_PATCH = "90f5e78f53e8f1a55bfa84adb6fb87cb";

const static string ZOMBIE_COD5_ASYLUM = "8e8cacd12035dc1042ba505c6ae53fb4";
const static string ZOMBIE_COD5_ASYLUM_PATCH = "f7ad43bd7dc7cb35158cf94c64941ac8";

const static string ZOMBIE_COD5_SUMPF = "5e58490d0fb6032bc93a489f1c5a001a";
const static string ZOMBIE_COD5_SUMPF_PATCH = "169e3d63b2e8b03f37fc57171720c099";

const static string ZOMBIE_COD5_FACTORY = "41c15a520ece8735261722e45f2734ad";
const static string ZOMBIE_COD5_FACTORY_PATCH = "4cadaa0fb9871347affaf3b222117859";

const static string GAME_MOD_FRONTEND = "06e3b0de38ef784d9bf3fd087187c502";
const static string GAME_MOD_DLL = "015c2fc598947895929ee629ae74bbe2";

const static string REMIX_MOD = "0eab4b4bb87c07a2bcf5343cc41377ad";
const static string FIRSTROOMS_MOD = "6f1c4a8bccad34e5ec783a5f16a55663";
const static string SONG_TIMING_MOD = "2962c5e032598b5888ae97df18313d76";

#endif