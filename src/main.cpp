#include <iostream>
#include <vector>
#include <thread>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

#include <boost/program_options.hpp>

// #include "helper.hpp"
#include "serial.h"
#include "console.h"

static bool exit_signal = false;

void signal_handler(int signo)
{
   if (signo == SIGINT || signo == SIGTERM){
      std::cout << "Exit signal catch" << std::endl;
      exit_signal = true;
   }
}


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


         // graceful shutdown  
         if (signal(SIGINT, signal_handler) == SIG_ERR)
            throw std::runtime_error("Cant catch SIGINT");

         if (signal(SIGTERM, signal_handler) == SIG_ERR)
            throw std::runtime_error("Cant catch SIGTERM");

         // init serial stream
         console cons;
         serial serial(port_path, prefix);
         serial.open();

         std::cout << "Current mode: " << mode << "\n";

         std::thread console_read_thread([&]{
            while(!exit_signal)
            {
               // get user input with 1s timeout
               const auto& line = cons.read(1000);
               if (line.size() == 0)
                  continue;

               // do stuff with input data
               try
               {
                  std::vector<std::uint8_t> bytes;
                  std::uint32_t num;

                  if (mode == "hex")
                  {
                     static const std::string snow_prefix = "0x"; 
                     // validate hex number
                     if(line.compare(0, snow_prefix.size(), snow_prefix) != 0)
                        throw std::runtime_error("Bad hex value: " + line);

                     // parse hex string to uint32
                     num = std::stoul(line, nullptr, 16);
                  }

                  if (mode == "dec")
                  {  
                     // parse dec string to uint32
                     num = std::stoul(line, nullptr, 10);
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
               catch(const std::exception& e)
               {
                  std::cerr << e.what();
               }
            }
            std::cout << "Exiting console read thread" << std::endl;
         });
         std::thread serial_read_thread([&]
         {
            while(!exit_signal)
            {
               const auto& bytes = serial.read();
               // parse string to values
               for(const auto& byte : bytes)
               {
                  std::cout << byte << std::endl;
               }
            }
            std::cout << "Exiting serial read thread" << std::endl;
         });

         console_read_thread.join();
         serial_read_thread.join();
         
         return 0;
      }
      catch(const std::exception &e)
      {
         std::cerr << e.what() << std::endl;
         return 1;
      }
}