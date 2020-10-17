#pragma once
#include <iostream>
#include <mutex>
#include <vector>
#include <string>

#include <libserial/SerialStream.h>

using namespace LibSerial;

class serial
{
public:
	serial(	const std::string& port_path, 
			const std::uint8_t& prefix);
	~serial();

	void open();

	void read();
	void write(std::vector<std::uint8_t>& bytes);

private:
	const std::string 	_port_path;
	const std::uint8_t 	_prefix;

	SerialStream 		_serial_port;
	std::mutex 			_mutex;
};