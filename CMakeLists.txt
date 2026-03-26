cmake_minimum_required(VERSION 3.14) # FetchContent 推荐 3.14+ 版本
project(EngineStudy LANGUAGES CXX)

# 1. 全局 C++ 标准配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. 编译选项：开启所有警告，把警告当错误（引擎岗严谨要求）
if(MSVC)
    # /W4: 开启等级4警告
    # /WX: 将警告视为错误
    # /utf-8: 强制编译器使用 UTF-8 编码解析源码
    add_compile_options(/W4 /WX /utf-8)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

# 3. 启用 CTest 自动化测试框架
enable_testing()

# 4. 第三方库管理 (FetchContent)
include(FetchContent)

# 抓取 Google Benchmark
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)
# 抓取时禁用 Benchmark 自带的测试，节省编译时间
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
# 触发
FetchContent_MakeAvailable(googlebenchmark)

# 5. 添加子模块 (注意顺序：先被依赖的，后依赖人的)
add_subdirectory(engine_core)
# add_subdirectory(math) # 准备好 math 文件夹后取消注释
add_subdirectory(tests)

# 6. 打印构建信息（可选，方便调试）
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")