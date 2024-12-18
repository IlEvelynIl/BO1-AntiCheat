#include "config.hpp"

#include <vector>

#include "../../game/game.hpp"

using namespace std;

namespace anticheat {
	namespace config {
		std::vector<std::string> disallowed_commands;

		// tell the anticheat what commands to look for
		void InitBadCommands()
		{
			disallowed_commands.push_back("cl_noprint");
			disallowed_commands.push_back("god");
			disallowed_commands.push_back("demigod");
			disallowed_commands.push_back("notarget");
			disallowed_commands.push_back("magic_chest_movable");
			disallowed_commands.push_back("ai_disableSpawn");
		}

		// reads the config for bad commands every map load
		bool CheckConfigForBadCommands()
		{
			std::string configPath = game::GetBlackOpsPath() + "\\players\\config.cfg";

		}

		// constantly looks at the console text to see if they're typing in a command
		bool CheckConsoleForBadCommands()
		{
			int address = 0x00C72299;

			// read the console text from the address
			// also note that the memory omits the first character
			// but they have to type in a slash to put those in anyhow, so yeah.
		}
	}
}