#pragma once
#include <format>
#include <source_location>
#include <string_view>
#include <cstdint>
#include <utility>
#include <vector>

namespace zen
{
    /**
    * @brief ログレベルを表す列挙型
    */
    enum class LogLevel : uint8_t
    {
        Trace = 0,
        Debug,
        Info,
        Warning,
        Error
    };

    class Logger
    {
    public:

        template<typename... Args>
        void log(
            const std::source_location& sourceLocation, LogLevel logLevel, const std::string_view message, Args&&... args)
        {
            if (isSuppressed())
            {
                return;
            }
            std::string buffer{};
            std::format_to(back_inserter(buffer), std::string_view(str), std::forward<Args>(args)...);
        }

        [[nodiscard]]
        bool isSuppressed(LogLevel logLevel) const noexcept
        {
            return  static_cast<uint8_t>(logLevel) < static_cast<uint8_t>(_logLevelFilter);
        }

        void setLogLevelFilter(LogLevel logLevelFilter) noexcept
        {
            _logLevelFilter = logLevelFilter;
        }

    private:
        LogLevel _logLevelFilter{ LogLevel::Info };
    };
}

#define ZEN_LOG(...)