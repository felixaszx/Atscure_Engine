#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <sstream>
#include <chrono>
#include <format>
#include <string>
#include <ctime>

namespace as
{
    class Log
    {
      private:
        inline static uint32_t total_logs_ = 0;

        template <typename T, typename... Args>
        void line_out(T first, Args... rest);

        template <typename T>
        void line_out(T first);

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
        Log(const R&... msg_rest);
    };

    /**
     * @brief IMPL
     **/

    template <typename T, typename... Args>
    inline void Log::line_out(T first, Args... rest)
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
    inline void Log::line_out(T first)
    {
        std::istringstream is(first);
        std::string line;
        while (std::getline(is, line))
        {
            std::cout << "| " << line << '\n';
        }
    }

    template <typename... R>
    inline Log::Log(const R&... msg_rest)
    {
        std::time_t now;
        std::time(&now);
        total_logs_++;
        std::cout << "\nv " << std::format("[{}] ", total_logs_) << std::ctime(&now);
        line_out(msg_rest...);
    }

};     // namespace as
#endif // LOGGING_HPP
