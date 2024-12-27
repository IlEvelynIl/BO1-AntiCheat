#include "settings.hpp"

#include <string>
#include <filesystem>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

namespace fs = std::filesystem;

namespace anticheat {
	namespace settings {
		std::map<std::string, std::string> settingValues;

		// saves the settings to the settings file
		void SaveSettings(bool _default)
		{
			std::string path = "bo1-anticheat.ini";
			std::ofstream file(path, std::ios::trunc);

			if (!file.is_open())
			{
				MessageBoxA(NULL, "Error opening \"bo1-anticheat.ini\"", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
				return;
			}

			// if we're saving the default settings, just write to the file
			if (_default)
			{
				file << "DisplayGuidelines=True\n";
			}
			else
			{
				for (const auto& [key, value] : settingValues)
				{
					file << key << "=" << value << "\n";
				}
			}

			file.close();
		}

		// loads settings from the settings file
		void LoadSettings()
		{
			std::string path = "bo1-anticheat.ini";

			if (!fs::exists(path))
			{
				SaveSettings(true);
				LoadSettings();
				return;
			}

			std::ifstream file(path);

			if (!file.is_open())
			{
				MessageBoxA(NULL, "Error opening \"bo1-anticheat.ini\"", "BO1-AntiCheat", MB_OK | MB_ICONERROR);
				return;
			}

			std::string line;
			while (std::getline(file, line))
			{
				if (line.empty() || line[0] == '#')
					continue;

				size_t delimiter = line.find('=');
				if (delimiter != std::string::npos)
				{
					std::string key = line.substr(0, delimiter);
					std::string value = line.substr(delimiter + 1);
					settingValues[key] = value;
				}
			}

			file.close();
		}

		std::string GetValue(std::string setting)
		{
			if (settingValues.find(setting) != settingValues.end())
			{
				return settingValues[setting];
			}

			return "";
		}
	}
}