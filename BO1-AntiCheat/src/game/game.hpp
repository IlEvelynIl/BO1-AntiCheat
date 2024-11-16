using std::string;

namespace game {
	int GetMapId();
	int GetGameTime();
	std::string GetZoneCommon();
	std::string GetZoneLanguage();
	std::string GetBlackOpsPath();

	// game mod
	void CheckForGameMod();
	bool IsGameModLoaded();
	bool IsGameModPresent();
	std::string GetModName();
	bool IsModLoaded();

	std::string GetLanguageFolder();
	std::string GetGameLanguage();
}