#include <Windows.h>
#include <string>

using namespace std;

#ifndef DisplayStatuses
#define DisplayStatuses

const static LPCWSTR TITLE = L"BO1 Anti Cheat v1.0";

const static string GAME_CONNECTED = "Game connected.";
const static string GAME_NOT_CONNECTED = "Game not connected.";
const static string WAITING_FOR_MAP_LOAD = "Waiting for a map load.";
const static string PERFORMING_SCANS = "Checking game integrity.";
const static string NO_CHEATING_DETECTED = "No known cheating methods detected.";
const static string CHEATING_DETECTED = "A form of cheating was detected:";

#endif