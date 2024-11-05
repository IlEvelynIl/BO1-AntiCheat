#include <string>

#include "../statuses.h"

using namespace std;

namespace anticheat {
	void Initialize();

	void AddScannableMaps();
	void OnGameClosed();
	void OnGameReopen();

	void AttemptIntegrityCheck();
	void OnCheatFound(std::string cheating_method);
	void NotifyCheatsDetected();

	void WaitForBlackOpsProcess();

	bool CheatingDetected();
	std::string GetMainStatus();
	std::string GetInfoStatus();
}