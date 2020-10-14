#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <iomanip>

#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>

#include <libserial/SerialStream.h>

static const char prefix = 0xDD;

class uint32_from_hex
{
    std::uint32_t value;
public:
    operator std::uint32_t() const { return value; }
    friend std::istream& operator>>( std::istream& in, uint32_from_hex& outValue )
    {
        in >> std::hex >> outValue.value;
        return in;
    }
};

std::string get_hex_value(const std::uint32_t& byte)
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

void send_bytes(const std::vector<std::uint32_t>& bytes)
{
   std::cout << "sending bytes..." << "\n";
   
   for (const auto& byte : bytes)
   {
      std::cout << get_hex_value(byte) << "\n";
      
   }
   std::cout << "bytes have sent!" << std::endl;
}

int main(int argc, const char** argv)
{
   using namespace LibSerial;
   using boost::lexical_cast;
   using boost::bad_lexical_cast;

   // init logger
    try
    {
         static const std::string null_byte = "0000";
         static const std::string snow_prefix = "0x"; 

         std::string mode;
         std::string log_level;
         std::string port_path;

         // Declare the supported options.
         boost::program_options::options_description desc("Allowed options");
         desc.add_options()
         ("help", "Shows this message")
         ("verbose", boost::program_options::value<std::string>(&log_level)->default_value("info"),  "Logging level (trace, debug, info, warning, error, fatal)") 
         ("port", boost::program_options::value<std::string>(&port_path)->default_value("/dev/ttyUSB0"),  "Port path.") 
         ("mode", boost::program_options::value<std::string>(&mode)->default_value("hex"),  "DEC or HEX.") 
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


         // SerialStream serial_stream;
         // serial_stream.Open(port_path);

         std::string input_line;
         while(std::cin) 
         {
            try
            {
               std::vector<std::uint32_t> bytes;
               std::uint32_t num;
               getline(std::cin, input_line);

               if (mode == "hex")
               {
                  // validate hex number
                  if(input_line.compare(0, snow_prefix.size(), snow_prefix) != 0)
                     throw std::runtime_error("Bad hex value: " + input_line);

                  // parse hex string to uint32
                  num = std::stoul(input_line, nullptr, 16);
               
               if (mode == "dec")
               {  
                  // parse dec string to uint32
                  num = std::stoul(input_line, nullptr, 10);
               }

               std::cout << "parsed value in dec: " << num << "\n";

               std::uint8_t *p = (std::uint8_t*)&num;

               // reorder bytes and add to vector
               for(int i=sizeof(num)-1; i>=0; i--)
                  {     
                     const std::uint32_t byte = (std::uint32_t)p[i];
                     if (byte)
                        bytes.emplace_back(byte);
                  }
               }

               send_bytes(bytes);
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

         // char write_byte_1 = 'a';
         // char write_byte_2 = 'b';

         // char read_byte_1 = 'A';
         // char read_byte_2 = 'B';

         // serial_stream << write_byte_2;

         // serial_stream >> read_byte_2;

         // std::cout << "serial_stream read: " << read_byte_2 << std::endl;

         // serial_stream.Close();
         return 0;
      }
      catch(const std::exception &e)
      {
         std::cerr << e.what() << std::endl;
         return 1;
      }
}