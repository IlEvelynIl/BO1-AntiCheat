#include <string>

using std::string;

class Display
{
public:
	void PreUpdateStatus(string newStatus);
	void UpdateStatus(string newStatus);
	void Update();
	void AddToStatus(string extraStatus);
};