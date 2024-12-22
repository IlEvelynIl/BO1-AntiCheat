#include <string>

#include <fstream>

#include <windows.h>

#include <iostream>

#include <filesystem>

#include "md5.h"

namespace utils {
	namespace files {
		std::string GetMD5(std::string file);
		int SizeInBytes(std::string file);
	}
}