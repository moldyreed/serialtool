#include "console.h"
#include <iostream>

#include <signal.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

std::string console::read(const time_t t)
{
	struct pollfd pfd = { STDIN_FILENO, POLLIN, 0 };
    std::string line;
    int ret = 0;
    
    ret = poll(&pfd, 1, t);     // timeout of 1000ms
    if(ret == 1)                // there is something to read
    {
        std::getline(std::cin, line);
    }
    else if(ret == -1)
    {
        std::cout << "Error: " << strerror(errno) << std::endl;
    }
    return line;
}