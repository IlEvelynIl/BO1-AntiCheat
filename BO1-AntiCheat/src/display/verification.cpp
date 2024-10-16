#include "verification.hpp"

using namespace std;

// shoutout lveez :D

#ifndef DEFINE_RAND1
#define DEFINE_RAND1 0
#endif

#ifndef DEFINE_RAND2
#define DEFINE_RAND2 0
#endif

unsigned verification_seed;

void Verification::init()
{
	auto now = chrono::system_clock::now();
	auto epoch_time = chrono::time_point_cast<chrono::seconds>(now).time_since_epoch();
	verification_seed = epoch_time.count();
}

void Verification::print_verification()
{
	auto seed = verification_seed;

	mt19937 engine1(DEFINE_RAND1 * seed);
	mt19937 engine2(DEFINE_RAND2 * seed);

	cout << "seed1: " << seed * DEFINE_RAND1 << "\n";
	cout << "uid: " << seed << " | " << engine1() << "\n";
	cout << "seed2: " << seed * DEFINE_RAND2 << "\n";
	cout << "uid: " << seed << " | " << engine2() << "\n\n";
}