#include <Windows.h>
#include <string>
#include <random>
#include <iostream>
#include <filesystem>
#include <map>

using std::string;

class Verification
{
public:
	void init();
	string GetUID_1();
	string GetUID_2();
};