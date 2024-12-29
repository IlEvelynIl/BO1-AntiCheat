namespace utils {
    namespace memory {
        int ReadInt(HANDLE processHandle, int address);
        std::string ReadString(HANDLE processHandle, int address);
    } // memory
} // utils