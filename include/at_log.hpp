#ifndef AT_LOG_HPP
#define AT_LOG_HPP

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

#include <fmt/core.h>

namespace ats
{
    enum class Result
    {
        SUCCESS,
        FAILURE,
        UNKNOWN,
        SUSPEND,
        PENDING
    };

    struct Log
    {

        static Result init();
        static void terminate();

        static void info(std::string infos);
        static void error(std::string errors);
        static void warrning(std::string warrnings);
    };

} // namespace ats

#endif // AT_LOG_HPP
