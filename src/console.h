#pragma once
#include <string>
#include <time.h>

class console
{
public:
	std::string read(const time_t t = 1000);
};