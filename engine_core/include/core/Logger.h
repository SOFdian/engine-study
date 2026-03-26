#pragma once
#include <string>

namespace core
{
    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Error
    };
    class Logger
    {
    public:
        // 基础打印函数
        static void Log(LogLevel level, const char *message, const char *file, int line);
    };

} // namespace core

// 宏封装：使用 __FILE__ 和 __LINE__ 自动捕获源码位置
#define LOG_INFO(msg) ::core::Logger::Log(::core::LogLevel::Info, msg, __FILE__, __LINE__)
#define LOG_WARN(msg) ::core::Logger::Log(::core::LogLevel::Warning, msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) ::core::Logger::Log(::core::LogLevel::Error, msg, __FILE__, __LINE__)
