using std::string;

class Memory
{
public:
    BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
    BOOL GetDebugPrivileges(void);

    int ReadInt(HANDLE processHandle, int address);
    bool WriteInt(HANDLE processHandle, int address, int value);

    int GetPointerAddress(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);
    int ReadPointerInt(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);

    float ReadFloat(HANDLE processHandle, int address);
    float ReadPointerFloat(HANDLE processHandle, int startAddress, int offsets[], int offsetCount);

    char* ReadString(HANDLE processHandle, int address);
    bool WriteString(HANDLE processHandle, int address, string value);
};