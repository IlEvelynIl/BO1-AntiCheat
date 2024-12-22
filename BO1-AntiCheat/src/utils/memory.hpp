namespace utils {
    namespace memory {
        int ReadInt(HANDLE processHandle, int address);
        bool WriteInt(HANDLE processHandle, int address, int value);
        char* ReadString(HANDLE processHandle, int address);
        bool WriteString(HANDLE processHandle, int address, char* value);
    } // memory
} // utils