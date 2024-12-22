#include "files.hpp"

using namespace std;

namespace utils {
	namespace files {
		std::string GetMD5(std::string file)
		{
            ifstream inFile(file, ios::binary);

            if (!inFile) {
                return "";
            }

            inFile.seekg(0, ios::end);
            long length = inFile.tellg();
            inFile.seekg(0, ios::beg);

            if (length <= 0) {
                return "";
            }

            vector<char> fileData(length);
            inFile.read(fileData.data(), length);

            if (!inFile) {
                return "";
            }

            string hash = md5(fileData.data(), length);
            return hash.c_str();
		}

        int SizeInBytes(std::string filePath)
        {
            std::ifstream file(filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                return -1;
            }

            return file.tellg();
        }
	}
}