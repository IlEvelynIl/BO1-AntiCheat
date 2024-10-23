#include "verification.hpp"

using namespace std;

// shoutout lveez :D

#ifndef DEFINE_RAND1
#define DEFINE_RAND1 0
#endif

#ifndef DEFINE_RAND2
#define DEFINE_RAND2 0
#endif

unsigned init_time;
unsigned seed1;
unsigned uid1;
unsigned seed2;
unsigned uid2;

void Verification::init()
{
	auto now = chrono::system_clock::now();
	auto epoch_time = chrono::time_point_cast<chrono::seconds>(now).time_since_epoch();
	init_time = epoch_time.count();

	mt19937 engine1(DEFINE_RAND1 * init_time);
	mt19937 engine2(DEFINE_RAND2 * init_time);

	seed1 = init_time * DEFINE_RAND1;
	uid1 = engine1();
	seed2 = init_time * DEFINE_RAND2;
	uid2 = engine2();
}

void Verification::print_verification()
{
	cout << "\n";
	cout << "seed: " << seed1 << "\n";
	cout << "uid1: " << init_time << " | " << uid1 << "\n";
	cout << "seed: " << seed2 << "\n";
	cout << "uid2: " << init_time << " | " << uid2 << "\n";
}