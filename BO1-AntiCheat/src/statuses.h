#include <Windows.h>
#include <string>

using namespace std;

#ifndef DisplayStatuses
#define DisplayStatuses

const static string GAME_CONNECTED = "Game connected.";
const static string GAME_NOT_CONNECTED = "Game not connected.";
const static string WAITING_FOR_MAP_LOAD = "Waiting for a map load.";
const static string PERFORMING_SCANS = "Checking game integrity.";
const static string NO_CHEATING_DETECTED = "No known cheating methods detected.";
const static string CHEATING_DETECTED = "A form of cheating was detected:";

const static LPCWSTR COULDNT_CHECK_UPDATES = L"There was a problem checking for updates.\nPlease try again later.";
const static LPCWSTR COULDNT_PROCESS_UPDATE = L"There was a problem processing the update data.\nPlease try again later.";
const static LPCWSTR NEW_UPDATE_AVAILABLE = L"A new update is available!\nWould you like to update now?";

#endif