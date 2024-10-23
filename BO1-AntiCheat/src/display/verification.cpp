#include "verification.hpp"

using namespace std;

// shoutout lveez :D

#ifndef DEFINE_RAND1
#define DEFINE_RAND1 0
#endif

#ifndef DEFINE_RAND2
#define DEFINE_RAND2 0
#endif

unsigned seed;
unsigned seed1;
unsigned uid1;
unsigned seed2;
unsigned uid2;

void Verification::init()
{
	auto now = chrono::system_clock::now();
	auto epoch_time = chrono::time_point_cast<chrono::seconds>(now).time_since_epoch();
	seed = epoch_time.count();

	mt19937 engine1(DEFINE_RAND1 * seed);
	mt19937 engine2(DEFINE_RAND2 * seed);

	seed1 = seed * DEFINE_RAND1;
	uid1 = engine1();
	seed2 = seed * DEFINE_RAND2;
	uid2 = engine2();
}

void Verification::print_verification()
{
	cout << "\n";
	cout << "seed: " << seed1 << "\n";
	cout << "uid1: " << seed << " | " << uid1 << "\n";
	cout << "seed: " << seed2 << "\n";
	cout << "uid2: " << seed << " | " << uid2 << "\n";
}