#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <string>
#include <chrono>

namespace as
{
    struct Log
    {
        static void info(const std::string& info);
        static void warnning(const std::string& warnning);
        static void error(const std::string& error);
    };
}; // namespace as

#endif // LOGGING_HPP
