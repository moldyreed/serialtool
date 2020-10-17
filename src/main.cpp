#include <iostream>
#include <vector>
#include <thread>
#include <arpa/inet.h>

#include <boost/program_options.hpp>

#include "serial.h"

int main(int argc, const char** argv)
{
   // init logger
    try
    {        
         std::string mode;
         std::string log_level;
         std::string port_path;
         std::uint8_t prefix;

         // Declare the supported options.
         boost::program_options::options_description desc("Allowed options");
         desc.add_options()
         ("help", "Shows this message")
         ("verbose", boost::program_options::value<std::string>(&log_level)->default_value("info"),  "Logging level (trace, debug, info, warning, error, fatal)") 
         ("port", boost::program_options::value<std::string>(&port_path)->default_value("/dev/pts/1"),  "Port path.") 
         ("mode", boost::program_options::value<std::string>(&mode)->default_value("hex"),  "DEC or HEX.") 
         ("prefix", boost::program_options::value<std::uint8_t>(&prefix)->default_value(0xDD),  "Prefix for data sending. From to 0 to 255, 1 byte.") 
         ;
         boost::program_options::variables_map vm;
         boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

         if (vm.count("help"))
         {
            std::cout << desc << std::endl;
            return 1;
         }
         boost::program_options::notify(vm);

         std::cout << "Current mode: " << mode << "\n";

         serial serial(port_path, prefix);
         serial.open();
         std::vector<std::uint8_t> read_bytes;
         std::thread read_thread(&serial::read, &serial);
         
         std::string input_line;
         while(std::cin) 
         {
            try
            {
               std::vector<std::uint8_t> bytes;
               std::uint32_t num;
               getline(std::cin, input_line);

               if (mode == "hex")
               {
                  static const std::string snow_prefix = "0x"; 
                  // validate hex number
                  if(input_line.compare(0, snow_prefix.size(), snow_prefix) != 0)
                     throw std::runtime_error("Bad hex value: " + input_line);

                  // parse hex string to uint32
                  num = std::stoul(input_line, nullptr, 16);
               }

               if (mode == "dec")
               {  
                  // parse dec string to uint32
                  num = std::stoul(input_line, nullptr, 10);
               }

               std::cout << "parsed value in dec: " << num << "\n";

               // reorder bytes
               num = htonl(num);
               std::uint8_t *p = (std::uint8_t*)&num;

               // vector
               for(int i=0; i<sizeof(num); i++)
               {     
                  const std::uint8_t& byte = (std::uint8_t)p[i];
                  if (byte){
                     bytes.emplace_back(byte);
                  }
               }

               serial.write(bytes);
            }
            catch(const std::runtime_error& e)
            {  
               throw;
            }
            catch(const std::exception& e)
            {
               std::cerr << e.what();
            }
         };

         read_thread.join();

         return 0;
      }
      catch(const std::exception &e)
      {
         std::cerr << e.what() << std::endl;
         return 1;
      }
}