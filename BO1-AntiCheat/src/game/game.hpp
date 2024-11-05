using std::string;

namespace game {

	void CheckGameMod();
	int GetMapId();
	bool IsGameOpen();
	std::string GetModName();
	bool IsModLoaded();
	bool IsGameModLoaded();
	bool CheckForGameModDLL();
	HANDLE GetBlackOpsProcess();
	void CloseBlackOps();
	std::string GetZoneCommon();
	std::string GetBlackOpsPath();
	std::string GetPathToExe();
	DWORD GetProcessIdByName(const std::wstring& procName);

}