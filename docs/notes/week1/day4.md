## Day 4：实现 Logger 基础版本

### 今天学什么

- 日志系统在工程中的作用
- 日志等级（Info / Warning / Error / Debug）
- 宏与函数封装的基本思路
- 为什么日志是调试基础设施的一部分

### 今天怎么学

1. 先不要设计复杂日志系统，只做“够用版本”。
2. 先明确日志最少要解决什么问题：
   - 输出信息
   - 标识等级
   - 辅助定位问题
3. 先写一个简单接口，再逐步增加格式与封装。
4. 在主程序中实际调用 Logger，观察输出效果。

### 今天要完成的任务

- 在 `engine_core` 中实现 Logger v1
- 至少支持：
  - `LOG_INFO`
  - `LOG_WARN`
  - `LOG_ERROR`
- 输出格式中至少包含：
  - 日志等级
  - 文本内容
- 在 `main.cpp` 中调用日志接口
- 记录 Logger 的设计思路

### 今天的产出物

- 可用的 Logger v1
- 演示日志输出的示例程序



=========================================================

```
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

```



**为什么必须用宏？** 如果直接调用 `Logger::Log()`，你在函数内部获取的 `__LINE__` 永远是 `Logger.cpp` 里的行号。通过宏展开，`__LINE__` 会在**调用处**（如 `main.cpp`）被替换，从而准确记录报错位置。

===================================================================

**`#define LOG_INFO(msg)`**：定义一个名为 `LOG_INFO` 的宏，它接受一个参数 `msg`。

**`::core::Logger::Log(...)`**：这是宏展开后的实际代码。使用 `::` 开头是为了确保从**全局命名空间**开始查找，防止在某些局部作用域下产生歧义。

**`__FILE__` 和 `__LINE__`**：这是编译器的**内置预定义宏**。

- `__FILE__`：在宏展开处替换为当前源文件的完整路径字符串。
- `__LINE__`：在宏展开处替换为当前代码所在的行号。

