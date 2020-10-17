#pragma once
#include <iomanip>
#include <vector>
#include <string>
#include <boost/algorithm/string/split.hpp>


std::string get_hex_value(const std::uint32_t byte)
{
   std::stringstream ss;
   ss <<             std::showbase << 
                     std::internal << 
                     std::setfill('0') << 
                     std::hex << 
                     std::setw(4) << 
                     (std::uint32_t)byte;
   return ss.str();
}

// std::vector<std::uint8_t> get_bytes_from_line(const std::string& line)
// {
// 	std::vector<std::uint8_t> bytes;
// 	// std::vector<std::string> strs;
// 	// boost::split(strs, line, boost::is_any_of(" "));
// 	// for (const auto& str : strs)
// 	// {
// 	// 	if (str.size() > 0)
// 	// 	{
//  //            // parse dec string to uint32
//  //            bytes.emplace_back(std::stoul(str, nullptr, 10));
// 	// 	}
// 	// }
// 	return bytes;
// } 