#include "display.hpp"

#include "verification.hpp"

#include "statuses.h"

#include "../utilities/memory.hpp"

#include "../game/game_handler.hpp"

#include "../constants.h"

using namespace std;

string status = DisplayStatuses::GAME_NOT_CONNECTED;

void Display::PreUpdateStatus(string newStatus)
{
	if (status == newStatus)
	{
		return;
	}

	status = newStatus;
}

void Display::UpdateStatus(string newStatus)
{
	if (status == newStatus)
	{
		return;
	}

	status = newStatus;
	Update();
}

void Display::Update()
{
	system("cls");
	cout << "============================================================================\n";

	cout << "       ____  ____ ___   ___          __  _    ________               __\n";
	cout << "      / __ )/ __ /  /  /   |  ____  / /_(_)  / ____/ /_  ___  ____ _/ /_\n";
	cout << "     / __  / / / / /  / /| | / __ \\/ __/ /  / /   / __ \\/ _ \\/ __ `/ __/\n";
	cout << "    / /_/ / /_/ / /  / ___ |/ / / / /_/ /  / /___/ / / /  __/ /_/ / /_\n";
	cout << "   /_____/\\____/_/  /_/  |_/_/ /_/\\__/_/   \\____/_/ /_/\\___/\\__,_/\\__/\n\n\n";

	cout << status << "\n";

	Memory mem;
	GameHandler gh;
	string mod_name = gh.GetModName();
	if (gh.IsGameModLoaded() && mod_name != "")
	{
		cout << "Mod: " << mod_name << "\n";
	}

	cout << "\n";

	Verification v;
	v.print_verification();

	cout << "============================================================================\n";
}

void Display::AddToStatus(string extraStatus)
{
	status += "\n" + extraStatus;
}