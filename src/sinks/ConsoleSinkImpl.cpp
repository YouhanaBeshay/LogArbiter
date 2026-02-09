#include "ConsoleSinkImpl.hpp"

#include <iostream> 

void ConsoleSinkImpl::write(const LogMessage &msg) {
    std::lock_guard<std::mutex> lock(consoleMutex_);
    
    // we use the operator overloaded <<
    std::cout << msg << std::endl;

    std::cout.flush();
}