#include "api/logging.hpp"

void as::Log::info(const std::string& info)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << fmt::format("\n[INFO] System time: {}{}\n\n", std::ctime(&end_time), info);
}

void as::Log::warnning(const std::string& warnning)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << fmt::format("\n[WARNINGS] System time: {}{}\n\n", std::ctime(&end_time), warnning);
}

void as::Log::error(const std::string& error)
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << fmt::format("\n[ERRORS] System time: {}{}\n\n", std::ctime(&end_time), error);
}
