using std::string;

class GameHandler
{
public:
	int GetMapId();
	bool IsGameOpen();
	string GetModName();
	bool IsModLoaded();
	bool IsGameModLoaded();
	HANDLE GetBlackOpsProcess();
	void CloseBlackOps();
	string GetZoneCommon();
	string GetBlackOpsPath();
	string GetPathToExe();
	DWORD GetProcessIdByName(const std::wstring& procName);
};