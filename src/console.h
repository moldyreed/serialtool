#pragma once
#include <string>
#include <time.h>

class console
{
public:
	std::string read(const time_t t = 1000);
 
	void write(const std::string& str);
	void operator<<(const std::string& str);
};