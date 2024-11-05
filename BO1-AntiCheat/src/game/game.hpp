using std::string;

namespace game {
	int GetMapId();
	std::string GetZoneCommon();
	std::string GetBlackOpsPath();

	// game mod
	void CheckForGameMod();
	bool IsGameModLoaded();
	bool IsGameModPresent();
	std::string GetModName();
	bool IsModLoaded();
}