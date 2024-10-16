using std::string;

class GameHandler
{
public:
	int GetMapId();
	bool IsGameOpen();
	HANDLE GetBlackOpsProcess();
	void CloseBlackOps();
	string GetZoneCommon();
	string GetBlackOpsPath();
	string GetProcessPathFromHandle(HANDLE handle);
	DWORD GetProcessIdByName(const std::wstring& procName);
	void CheckIfOpen();
	void OnGameClose();
	bool GameAlreadyOpen();
};