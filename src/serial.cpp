#include "serial.h"
#include "helper.hpp"

#include <unistd.h>

serial::serial(	const std::string& port_path,
				const std::uint8_t& prefix) :
	_port_path(port_path),
	_prefix(prefix)
{
}

serial::~serial()
{
	_serial_port.Close();
}

void serial::open()
{
	_serial_port.Open(_port_path);
}

void serial::read()
{
	// std::vector<std::uint32_t> nums;
	// while(true)
	// {
	// 	std::string line;
	// 	// wait for \r data
	// 	std::getline(_serial_port, line, '\r');
	// 	std::vector<std::string> numbers;
	// 	boost::split(numbers, line, [](char c){return c == ' ';});
	// }
	// std::vector<std::uint32_t> nums;
	// while(true)
	// {
	// 	std::uint32_t byte;
	// 	_serial_port >> byte;
	// 	std::cout << get_hex_value(byte) << ' ';
	// 	if (_serial_port.rdbuf()->in_avail() == 0)
	// 		std::cout << std::endl;
	// }

	std::vector<std::uint32_t> nums;
	while(true)
	{
		std::uint32_t byte;
		_serial_port >> byte;
		std::cout << "Got uncome byte: " << get_hex_value(byte) << ' ';
	}
}

void serial::write(std::vector<std::uint8_t>& bytes)
{
	std::cout << "sending " << bytes.size() << " bytes..." << "\n";

	_serial_port << (0xFF & _prefix) << ' ';

	for (const auto& byte : bytes)
	{
	  _serial_port << (0xFF & byte) << ' ';
	  std::cout << get_hex_value(byte) << ' ';
	}
	std::cout << "\n";

	// return carret and new line
	_serial_port << "\r\n";
	_serial_port.DrainWriteBuffer();
	std::cout << "bytes have sent!" << std::endl;
}
