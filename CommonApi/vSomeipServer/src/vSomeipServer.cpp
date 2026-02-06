#include "GpuUsageDataStubImpl.hpp"

#include <CommonAPI/CommonAPI.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();

    const std::string &domain = "local";
    const std::string &instance = "logarbiter.gpu.GpuUsageData";

    auto stub = std::make_shared<GpuUsageDataStubImpl>();
    
    bool success = runtime->registerService(domain, instance, stub);
    while (!success)
    {
        std::cout << "Failed to register service ,will try again" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        success = runtime->registerService(domain, instance, stub);
    }

    std::cout << "Service registered" << std::endl;
    
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
