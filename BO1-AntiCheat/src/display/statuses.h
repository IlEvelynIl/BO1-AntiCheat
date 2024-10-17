#include <Windows.h>
#include <string>

using namespace std;

#ifndef DisplayStatuses
#define DisplayStatuses

const static LPCWSTR TITLE = L"BO1 Anti Cheat v1.0";

const static char* GAME_CONNECTED = "Game connected.";
const static char* GAME_NOT_CONNECTED = "Game not connected.";
const static char* GAME_ALREADY_OPEN = "Call of Duty: Black Ops must not be open beforehand. Please reopen it.";
const static char* WAITING_FOR_MAP_LOAD = "Waiting for a map load.";
const static char* PERFORMING_SCANS = "Checking game integrity.";
const static char* NO_CHEATING_DETECTED = "No known cheating methods detected.";
const static char* CHEATING_DETECTED = "A form of cheating was detected.";

#endif