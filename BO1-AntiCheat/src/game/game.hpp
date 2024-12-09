using std::string;

namespace game {
	int GetMapId();
	int GetGameTime();
	std::string GetZoneCommon();
	std::string GetZoneLanguage();
	std::string GetBlackOpsPath();

	void CheckForAllowedTools();

	// game mod
	bool IsGameModLoaded();
	bool IsGameModPresent();
	std::string GetModName();
	bool IsModLoaded();

	// custom fx tool
	bool IsCustomFxToolLoaded();
	bool IsCustomFxToolPresent();

	std::string GetLanguageFolder();
	std::string GetGameLanguage();
}