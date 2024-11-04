using std::string;

class GameHandler
{
public:
	void CheckGameMod();
	int GetMapId();
	bool IsGameOpen();
	string GetModName();
	bool IsModLoaded();
	bool IsGameModLoaded();
	bool CheckForGameModDLL();
	HANDLE GetBlackOpsProcess();
	void CloseBlackOps();
	string GetZoneCommon();
	string GetBlackOpsPath();
	string GetPathToExe();
	DWORD GetProcessIdByName(const std::wstring& procName);
};