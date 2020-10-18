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
	std::cout << "closing seria port";
}

void serial::open()
{
	_serial_port.Open(_port_path);
}

std::string serial::readline()
{
	const auto& bytes = read();
	return std::string{bytes.cbegin(), bytes.cend()};
}

std::vector<std::uint8_t> serial::read()
{
	std::vector<std::uint8_t> bytes;
	char byte;
	while(_serial_port.get(byte))
	{
		if (byte == 0x0D || byte == 0x0A || byte == 0x03) // \r or \n or end of text
			break;
		bytes.emplace_back((std::uint8_t)byte);
	}
	return bytes;
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
