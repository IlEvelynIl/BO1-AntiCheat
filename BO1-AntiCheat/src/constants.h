#include <windows.h>

#ifndef Constants
#define Constants

const static std::string VERSION = "1.31";
const static LPCWSTR UPDATER_TITLE = L"BO1 Anti Cheat (Updater)";

// addresses
const static int C_MAPADDRESS = 0x02F67B6C;
const static int C_MODADDRESS = 0x025FE010;
const static int C_GAMEPAUSED = 0x8902B4;

// map ids
const static int MAIN_MENU_ID = 5;

#endif