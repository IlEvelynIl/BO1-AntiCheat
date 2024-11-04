#include <Windows.h>
#include <string>
#include <random>
#include <iostream>
#include <filesystem>
#include <map>

using std::string;

namespace anticheat {
	namespace verification {
		void CalculateUIDs();
		string GetUID_1();
		string GetUID_2();
	}
}