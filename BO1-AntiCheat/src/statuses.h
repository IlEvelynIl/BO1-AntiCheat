#include <Windows.h>
#include <string>

using namespace std;

#ifndef Statuses
#define Statuses

const static string GAME_CONNECTED = "Game connected";
const static string GAME_NOT_CONNECTED = "Game not connected";
const static string WAITING_FOR_MAP_LOAD_QUIT = "Waiting for a map load/quit";
const static string WILL_CHECK_NEXT_MAP_LOAD = "Will check for patches next map load/quit";
const static string CHECKING_FOR_PATCHES = "Checking for known patching methods";
const static string NO_PATCHING_DETECTED = "No known patching methods detected";
const static string WILL_CONTINUE_SEARCH = "The anti cheat will keep scanning for cheats during the game";
const static string CHEATING_DETECTED = "A known cheating method was detected";
const static string MORE_INFO_WINDOW = "More information can be found on the detections window";
const static string PATCHES_CHECKED_AFTER_MAP_LOAD = "Patching checks were run after the map was loaded";

const static string COULDNT_CHECK_UPDATES = "There was a problem checking for updates.\nPlease try again later.";
const static string COULDNT_PROCESS_UPDATE = "There was a problem processing the update data.\nPlease try again later.";
const static string NEW_UPDATE_AVAILABLE = "A new update is available! Would you like to update now?";

#endif