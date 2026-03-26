#include <core/Assert.h>
#include <cstdlib>

#if defined(_MSC_VER)
#include <intrin.h> // Windows 专用的中断指令
#endif

namespace core
{
    namespace Internal
    {
        void DebugBreak()
        {
            #if defined(_MSC_VER)
                __debugbreak(); // 在 VS 中会直接跳到出事代码行
            #else
                std::abort(); // 其他平台简单的退出
            #endif
        }
    }
}