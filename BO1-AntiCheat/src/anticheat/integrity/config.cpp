#include "config.hpp"

#include "../../game/game.hpp"

#include "../../utils/strings.hpp"

#include "../../utils/files.hpp"

using namespace std;

namespace anticheat {
	namespace integrity {
		namespace config {
			int last_config_size = 0;
			std::vector<ConfigDvar> cheating_commands;

			// tells the anticheat the commands to search for and how to search for them.
			void Initialize()
			{
				ConfigDvar cl_noprint = { "cl_noprint" };
				ConfigDvar magic_chest_movable = { "magic_chest_movable" };
				ConfigDvar ai_disableSpawn = { "ai_disableSpawn" };
				ConfigDvar difficultyEasy = { "difficultyEasy" };
				ConfigDvar difficultyMedium = { "difficultyMedium" };
				ConfigDvar difficultyHard = { "difficultyHard" };
				ConfigDvar difficultyFu = { "difficultyFu" };
				ConfigDvar zombie_taunt_freq = { "zombie_taunt_freq" };
				ConfigDvar zombie_reachin_freq = { "zombie_reachin_freq" };
				ConfigDvar scr_force_weapon = { "scr_force_weapon" };
				ConfigDvar scr_force_quantum_bomb_result = { "scr_force_quantum_bomb_result" };

				ConfigDvar cg_hudDamageIconTime = { "cg_hudDamageIconTime", "seta cg_hudDamageIconTime \"2000\"" };

				cheating_commands = {
					cl_noprint,
					magic_chest_movable,
					ai_disableSpawn,
					difficultyEasy,
					difficultyMedium,
					difficultyHard,
					difficultyFu,
					zombie_taunt_freq,
					zombie_reachin_freq,
					scr_force_weapon,
					scr_force_quantum_bomb_result,
					cg_hudDamageIconTime
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

					for (ConfigDvar dvar : cheating_commands)
					{
						std::string dvar_name = dvar.name;
						std::string dvar_name_lower = utils::strings::ToLower(dvar.name);
						bool contains_command = utils::strings::Contains(line_lower, dvar_name_lower);

						if (contains_command)
						{
							// we have to make sure the default values arent tampered with:
							if (dvar.expected_line != "")
							{
								if (line_lower != utils::strings::ToLower(dvar.expected_line))
								{
									config_commands.push_back(dvar_name + " (Value changed)");
								}
							}
							// for lines that should not exist at all
							else {
								config_commands.push_back(dvar_name);
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