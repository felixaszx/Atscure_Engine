#include "at_log.hpp"

namespace ats
{
    Result Log::init()
    {
        return Result::SUCCESS;
    }

    void Log::terminate()
    {
    }

    void Log::info(std::string infos)
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << fmt::format("[INFO] System time: {} {} \n", std::ctime(&end_time), infos);
    }

    void Log::error(std::string errors)
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << fmt::format("[ERRORS] System time: {} {}\n", std::ctime(&end_time), errors);
    }

    void Log::warrning(std::string warrnings)
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << fmt::format("[WARNINGS] System time: {} {}\n", std::ctime(&end_time), warrnings);
    }
}; // namespace ats
