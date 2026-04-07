# ---------------------------------------------------------------------------
# 1. 单元测试 (Unit Tests) - 使用 doctest
# ---------------------------------------------------------------------------
add_executable(unit_tests 
    src/unit_tests.cpp
)

# 链接 engine_core 库
target_link_libraries(unit_tests 
    PRIVATE 
        engine_core
)

# 指定 doctest 的头文件路径（假设你放在了根目录的 third_party 下）
target_include_directories(unit_tests 
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/third_party/doctest
)

# 注册到 CTest 系统，这样你可以通过命令行输入 'ctest' 运行所有测试
add_test(NAME EngineUnitTests COMMAND unit_tests)

# ---------------------------------------------------------------------------
# 2. 基准测试 (Benchmarks) - 使用 Google Benchmark
# ---------------------------------------------------------------------------
add_executable(bench_tests 
    src/benchmarks.cpp
)

# 链接到 googlebenchmark 导出的目标名
# 注意：根目录的 FetchContent_MakeAvailable(googlebenchmark) 已经让这个目标全局可用了
target_link_libraries(bench_tests 
    PRIVATE 
        engine_core
        benchmark::benchmark
)

# ---------------------------------------------------------------------------
# 3. (可选) 如果有数学库测试，可以继续添加
# ---------------------------------------------------------------------------
# add_executable(math_tests math_tests.cpp)
# target_link_libraries(math_tests PRIVATE math engine_core)
# add_test(NAME MathTests COMMAND math_tests)


add_executable(make_unique
    src/MakeUnique.cpp
)

target_link_libraries(make_unique
    PRIVATE
        engine_core
)


add_executable(test_pointers
    src/test_pointers.cpp
)

target_link_libraries(test_pointers
    PRIVATE
        engine_core
)

add_executable(test_alignment
    src/test_alignment.cpp
)

target_link_libraries(test_alignment
    PRIVATE
        engine_core
)

add_executable(test_cache
    src/test_cache.cpp
)

target_link_libraries(test_cache
    PRIVATE
        engine_core
)

add_executable(test_aos_soa
    src/test_aos_soa.cpp
)

target_link_libraries(test_aos_soa
    PRIVATE
        engine_core
)

add_executable(test_false_sharing
    src/test_false_sharing.cpp
)

target_link_libraries(test_false_sharing
    PRIVATE
        engine_core
)