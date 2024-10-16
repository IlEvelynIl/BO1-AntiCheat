#include <Windows.h>
#include <string>

using namespace std;

#ifndef DisplayStatuses
#define DisplayStatuses

const static LPCWSTR TITLE = L"BO1 AntiCheat v1.0";

const static char* GAME_CONNECTED_WAITING = "Game connected. Waiting for map to scan.";
const static char* GAME_NOT_CONNECTED = "Game not connected.";
const static char* GAME_ALREADY_OPEN = "Call of Duty: Black Ops must not be open beforehand. Please reopen it.";
const static char* PERFORMING_SCANS = "Scanning game fastfiles/patches.";
const static char* NO_CHEATING_DETECTED = "No fastfile/patch modification detected.";
const static char* CHEATING_DETECTED = "A form of cheating was detected.";

#endif