#include <string>

#include "../statuses.h"

using namespace std;

namespace anticheat {
	void Initialize();
	void OnGameClosed();
	void AttemptIntegrityCheck();
	void AddCheatFound(std::string cheating_method);
	void NotifyCheatsDetected();
	void WaitForBlackOpsProcess();

	bool CheatingDetected();
	std::string GetMainStatus();
	std::string GetInfoStatus();
}