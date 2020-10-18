#pragma once

#include <iomanip>
#include <vector>
#include <string>
#include <bitset>

#include <boost/algorithm/string/split.hpp>

static std::string get_hex_value(const std::uint32_t byte)
{
   std::stringstream ss;
   ss <<             std::showbase << 
                     std::internal << 
                     std::setfill('0') << 
                     std::hex << 
                     std::setw(4) << 
                     byte;
   return ss.str();
}

static std::string get_bin_value(const std::uint32_t byte)
{
   std::stringstream ss;
   ss << std::bitset<32>(byte);
   return ss.str();
}