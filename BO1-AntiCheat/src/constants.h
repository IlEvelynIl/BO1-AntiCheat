#include <windows.h>

#ifndef Constants
#define Constants

const static std::string VERSION = "2.1.2";
const static std::string GUIDELINES = "- Show the BO1-AntiCheat.exe file properties.\n- Show yourself opening the tool.\n- Display the tool on your stream/recording.";

// addresses
const static int C_MAPADDRESS = 0x02F67B6C;
const static int C_TIMEADDRESS = 0x01A79870;
const static int C_MODADDRESS = 0x025FE010; // only on game_mod
const static int C_LANGADDRESS = 0x0276C0D0;
const static int C_GAMEPAUSED = 0x8902B4;

// map ids
const static int MAIN_MENU_ID = 5;

#endif