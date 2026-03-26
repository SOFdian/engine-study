#pragma once
#include <core/Logger.h>

#ifdef NDEBUG
#define CORE_ASSERT(condition, msg) ((void)0)
#else
// 这里的逻辑：如果条件为假，则打印错误并触发中断
#define CORE_ASSERT(condition, msg)                                                                 \
    if (!(condition))                                                                               \
    {                                                                                               \
        ::core::Logger::Log(::core::LogLevel::Error, "ASSERTION FAILED: " msg, __FILE__, __LINE__); \
        ::core::Internal::DebugBreak();                                                             \
    }
#endif

namespace core
{
    namespace Internal
    {
        // 触发调试器中断的平台无关封装
        void DebugBreak();
    }
}