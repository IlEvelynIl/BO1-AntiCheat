#include <string>

namespace anticheat {
	namespace config {
		void InitBadCommands();
		bool CheckConfigForBadCommands(); // read every map start
		bool CheckConsoleForBadCommands(); // read every 50ms
	}
}