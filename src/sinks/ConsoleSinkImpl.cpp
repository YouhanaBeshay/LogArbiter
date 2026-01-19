#include "ConsoleSinkImpl.hpp"

#include <iostream> 

void ConsoleSinkImpl::write(const LogMessage &msg) {
    
    // we use the operator overloaded <<
    std::cout << msg << std::endl;
}