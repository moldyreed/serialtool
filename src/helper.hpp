#pragma once
#include <iomanip>
#include <vector>

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

// std::vector<std::uint32_t> parse_digits(const std::string& str)
// {

// }