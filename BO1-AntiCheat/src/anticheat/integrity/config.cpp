#include "config.hpp"

#include "../../game/game.hpp"

#include "../../utils/strings.hpp"

#include "../../utils/files.hpp"

using namespace std;

namespace anticheat {
	namespace integrity {
		namespace config {
			int last_config_size = 0;
			std::map<std::string, std::string> cheating_commands;

			// tells the anticheat the commands to search for and how to search for them.
			//	types:
			//		all = any instance on any line
			//		bind = only lines that start with "bind"
			//		seta = only lines that start with "seta"
			void Initialize()
			{
				cheating_commands = {
					make_pair("cl_noprint", "all"),
					make_pair("magic_chest_movable", "all"),
					make_pair("ai_disableSpawn", "all"),
					make_pair("difficultyEasy", "all"),
					make_pair("difficultyMedium", "all"),
					make_pair("difficultyHard", "all"),
					make_pair("difficultyFu", "all"),
					make_pair("zombie_taunt_freq", "all"),
					make_pair("zombie_reachin_freq", "all"),
					make_pair("scr_force_weapon", "all"),
					make_pair("scr_force_quantum_bomb_result", "all")
				};
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

					for (const auto& [key, value] : cheating_commands)
					{
						std::string command = key;
						std::string type = value;
						bool contains_command = utils::strings::Contains(line_lower, command);

						// handle every instance
						if (type == "all")
						{
							if (contains_command)
							{
								config_commands.push_back(command);
								continue;
							}
						}
						// bind only instances
						else if (type == "bind" && utils::strings::StartsWith(line_lower, "bind"))
						{
							if (contains_command)
							{
								config_commands.push_back(command);
								continue;
							}
						}
						// seta only instances
						else if (type == "seta" && utils::strings::StartsWith(line_lower, "seta"))
						{
							if (contains_command)
							{
								config_commands.push_back(command);
								continue;
							}
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
				return false;
			}
		} // config
	} // integrity
} // anticheat