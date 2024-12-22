#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>

namespace anticheat {
	namespace integrity {
		namespace config {
			void Initialize();
			std::string GetCheatingCommands();
			bool CheckConsoleForBadCommands(); // read every 50ms
		} // config
	} // integrity
} // anticheat