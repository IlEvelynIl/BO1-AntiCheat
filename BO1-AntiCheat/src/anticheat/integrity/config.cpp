#include "config.hpp"

#include "../../game/game.hpp"

#include "../../utils/strings.hpp"

#include "../../utils/files.hpp"

using namespace std;

namespace anticheat {
	namespace integrity {
		namespace config {
			int last_config_size = 0;
			std::vector<std::string> cheating_commands;

			// tell the anticheat what commands to look for
			void Initialize()
			{
				cheating_commands.push_back("cl_noprint");
				cheating_commands.push_back("magic_chest_movable");
				cheating_commands.push_back("ai_disableSpawn");
				cheating_commands.push_back("difficultyEasy");
				cheating_commands.push_back("difficultyMedium");
				cheating_commands.push_back("difficultyHard");
				cheating_commands.push_back("difficultyFu");
			}

			// reads the config for cheating commands every map load
			std::string GetCheatingCommands()
			{
				std::string config_path = game::GetBlackOpsPath() + "\\players\\config.cfg";
				if (game::IsModLoaded())
				{
					config_path = game::GetBlackOpsPath() + "\\players\\" + game::GetModName() + "\\config.cfg";
				}
				
				// check if the config exists
				if (!filesystem::exists(config_path))
				{
					return "";
				}

				// check the size of the config and store it, prevents constant file readers
				int config_bytes = utils::files::SizeInBytes(config_path);

				// something went wrong with reading the file
				if (config_bytes == -1)
				{
					return "";
				}

				// nothing has changed
				if (config_bytes == last_config_size)
				{
					return "";
				}

				// set the last config size - we're reading it.
				last_config_size = config_bytes;

				std::ifstream file(config_path);
				if (!file.is_open())
				{
					return "";
				}

				vector<string> config_commands;

				// go through each line
				std::string line;
				while (std::getline(file, line)) {
					std::string line_lower = utils::strings::ToLower(line);

					// go through the commands that are considered cheating
					for (size_t i = 0; i < cheating_commands.size(); i++)
					{
						std::string command = cheating_commands[i];

						// check for a lowercase match on lowercase lines
						bool contains_command = utils::strings::Contains(line_lower, command);
						if (contains_command)
						{
							config_commands.push_back(command);
						}
					}
				}

				file.close();

				// format all cheating commands into a nice list
				string formatted_commands = "";
				for (int i = 0; i < config_commands.size(); i++)
				{
					string command = config_commands[i];
					formatted_commands += command;
					if (i != config_commands.size() - 1)
					{
						formatted_commands += ", ";
					}
				}

				return formatted_commands;
			}

			// constantly looks at the console text to see if they're typing in a command
			bool CheckConsoleForBadCommands()
			{
				int address = 0x00C72299;

				// read the console text from the address
				// also note that the memory omits the first character
				// but they have to type in a slash to put those in anyhow, so yeah.
			}
		} // config
	} // integrity
} // anticheat