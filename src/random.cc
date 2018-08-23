#include "random.h"

using namespace std;

std::random_device Random::dev;

std::mt19937 Random::g(Random::dev());

unsigned int Random::seed;
