#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <sstream>
#include <chrono>
#include <format>
#include <string>
#include <ctime>

namespace ae
{
    class Log
    {
      private:
        template <typename T, typename... Args>
        void line_out(T first, Args... rest)
        {
            std::istringstream is(first);
            std::string line;
            while (std::getline(is, line))
            {
                std::cout << "| " << line << '\n';
            }
            line_out(rest...);
        }

        template <typename T>
        void line_out(T first)
        {
            std::istringstream is(first);
            std::string line;
            while (std::getline(is, line))
            {
                std::cout << "| " << line << '\n';
            }
        }

      public:
        enum Level
        {
            INFO,
            ERROR,
            WARNNING,
            CRITICAL,
            VALIDATION
        };

        template <typename... R>
        Log(const R&... msg_rest)
        {
            std::time_t now;
            std::time(&now);
            std::cout << "\nv " << std::ctime(&now);
            line_out(msg_rest...);
        }
    };
}; // namespace ae

#endif // LOGGING_HPP