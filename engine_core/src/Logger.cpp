#include "core/Logger.h"
#include <iostream>
#include <vector>

namespace core {

void Logger::Log(LogLevel level, const char* message, const char* file, int line) {
    const char* levelStr = "INFO";
    switch (level) {
        case LogLevel::Debug:   levelStr = "DEBUG"; break;
        case LogLevel::Info:    levelStr = "INFO";  break;
        case LogLevel::Warning: levelStr = "WARN";  break;
        case LogLevel::Error:   levelStr = "ERROR"; break;
    }

    // 格式化输出：[级别] 信息 (文件名:行号)
    std::cout << "[" << levelStr << "] " << message 
              << " (" << file << ":" << line << ")" << std::endl;
}

} // namespace core