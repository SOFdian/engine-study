

## Day 6：接入单元测试与 benchmark 框架

### 今天学什么

- 为什么第一周就要接测试
- 单元测试的最小实践方式
- benchmark 的意义
- “能运行”和“可验证”之间的区别

### 今天怎么学

1. 选择一个轻量测试框架和 benchmark 框架。
2. 不要求把所有模块都覆盖，重点是把流程跑通。
3. 先给 Logger、Assert 或一个简单工具函数写 1~2 个测试。
4. benchmark 先测一个很简单的函数，只为熟悉流程。

### 今天要完成的任务

- 接入测试框架
- 在 `tests/` 中写至少 2 个测试用例
- 保证测试可通过 CMake 运行
- 接入 benchmark 框架
- 写一个最简单 benchmark 示例

### 今天的产出物

- 测试 target
- 至少 2 个单元测试
- 至少 1 个 benchmark 示例

==========================================================

1. **单元测试**：使用 **doctest**。它是目前最快的单头文件 C++ 测试框架，接入极其简单。
2. **基准测试**：使用 **Google Benchmark**。它是行业标准，能精确测量代码的执行耗时。



## doctest下载

github搜到.h文件放到third_party里就可以了

=============================================================================

## 通过 CMake `FetchContent` 自动拉取 Google Benchmark

```
include(FetchContent)

# 抓取 Google Benchmark
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)

# 抓取时禁用 Benchmark 自带的测试，节省编译时间
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)
```

其中最后一行的作用是，让这个目标全局可用

1.**检查本地**：看 `build/_deps/` 目录下是否已经有源码。

2.**下载/更新**：如果没有，就去 GitHub 按照你指定的 `v1.8.3` 标签下载。

3.**集成 (Add Subdirectory)**：自动执行类似 `add_subdirectory()` 的操作，将下载的源码纳入当前的编译树。

4.通过benchmark::benchmark即可调用（这是在`goolebenchmark`里写死的，里面的CML会有类似add_library(benchmark ...) 这样的代码）

=============================================================================

## CMake测试框架

```
add_executable(unit_tests 
    src/unit_tests.cpp
)
.......

add_test(NAME EngineUnitTests COMMAND unit_tests)
```

EngineUnitTests为这个测试任务的名称

unit_tests则是告诉CTest应该启动哪个程序来运行测试

### 判定逻辑

**ctest 的判定**： `ctest` 启动 `unit_tests.exe` 后会一直“盯着”这个进程。

- 它看到程序返回了 **0**，就在控制台打出绿色的 `Passed`。
- 它看到返回了 **非零**，就打出红色的 `Failed`。

### 输入`ctest`这一命令时的整体运行逻辑

1. **查找**：CTest 在清单中找到一个名为 `EngineUnitTests` 的条目。
2. **调用**：它查看该条目的 `COMMAND`，发现对应的程序是 `unit_tests`。
3. **运行**：CTest 启动 `unit_tests.exe` 进程。
4. **反馈**：
   - 如果 `unit_tests.exe` 里的代码（比如你的 `doctest` 断言）全部通过，程序最后会 `return 0`。
   - CTest 看到返回值为 **0**，就在控制台打出一个绿色的 **Passed**。

=============================================================================

## doctest

```
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <core/Logger.h>

// 模拟一个简单的待测功能：假设我们要测试数学库（Day 11会深入）
int Addition(int a, int b) { return a + b; }

TEST_CASE("Testing basic engine utilities") {
    CHECK(Addition(1, 1) == 2);
    CHECK(Addition(-1, 1) == 0);
}

TEST_CASE("Logger Integrity Check") {
    // 验证 LOG 宏是否会引起崩溃
    LOG_INFO("Testing logger from unit test...");
    CHECK(true); 
}
```

**doctest 的行为**： 当你运行 `unit_tests.exe` 时，`doctest` 会扫描所有测试用例。

- 如果所有断言（Assertions）都通过，`doctest` 会让程序以 **状态码 0** 结束。
- 如果只要有一个断言失败，`doctest` 就会让程序以 **非零状态码（通常是 1）** 结束。

=====================================================================================

## Benchmark

```
#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>

// 测试 std::sort 对不同长度向量的性能
static void BM_SortVector(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        // 使用 static_cast 明确转换类型，消除 C4244 警告
        auto range_size = static_cast<size_t>(state.range(0)); 
        std::vector<int> v(range_size, 1);
        
        std::generate(v.begin(), v.end(), std::rand);
        state.ResumeTiming();

        std::sort(v.begin(), v.end());
        benchmark::DoNotOptimize(v.data());
    }
}

// 运行不同数据规模的测试：8, 64, 512
BENCHMARK(BM_SortVector)->Range(8, 512);

BENCHMARK_MAIN();
```

### 1. 运行入口：`BENCHMARK_MAIN()`

当你运行编译出的 `bench_tests.exe` 时，程序会进入 `BENCHMARK_MAIN()` 宏定义的入口。 这个宏会启动 Google Benchmark 的运行时环境，它会自动扫描所有通过 `BENCHMARK(...)` 注册的函数，并依次执行它们。

------

### 2. 核心逻辑：自动迭代与统计稳定

Benchmark 并不只是简单地把你的代码跑一遍。为了消除操作系统抖动和 CPU 频率波动的影响，它遵循以下步骤：

- **预热 (Warm-up)**：在正式计时前，框架会先运行几次你的代码。这能让 CPU 进入高频状态，并将相关指令加载到指令缓存（L1i Cache）中，避免“冷启动”导致的数据偏差。
- **动态迭代 (Dynamic Iterations)**：框架会启动一个 `for (auto _ : state)` 循环。它会先跑 100 次，如果耗时太短无法精确测量，它会自动增加到 1,000 次、10,000 次甚至更多，直到样本量足以支撑稳定的统计结果。
- **消除优化 (Preventing Optimization)**：编译器非常聪明，如果你在循环里做了一些没用的计算，它可能会把你的代码“优化掉”。因此我们必须使用 `benchmark::DoNotOptimize(...)` 来强制编译器保留这些计算逻辑，确保我们测的是真东西。

### 3. 结果产出：多维度耗时分析

运行结束后，你会看到控制台输出一个表格：

| **任务名称 (Benchmark)** | **迭代次数 (Iterations)** | **实际耗时 (Time)** | **CPU 耗时 (CPU)** |
| ------------------------ | ------------------------- | ------------------- | ------------------ |
| `BM_SortVector/8`        | 2345678                   | **15 ns**           | **15 ns**          |
| `BM_SortVector/64`       | 890123                    | **120 ns**          | **119 ns**         |

- **Time (Wall Time)**：程序在墙上时钟流逝的时间，包含等待 IO 或线程调度的时间。
- **CPU Time**：CPU 真正花费在这个进程上的时间。如果这两个数字差距很大，说明你的代码可能存在严重的阻塞或上下文切换。

------

### 4. 为什么不建议用 `ctest` 跑 Benchmark？

虽然技术上可行，但在引擎开发中我们通常将它们分开：

1. **干扰测量**：`ctest` 本身也是一个进程，它在后台监控和捕获输出会占用一定的系统资源，这可能会污染 Benchmark 的微秒级测量结果。
2. **目标不同**：`ctest` 只要看到“0”就觉得成功了；而 Benchmark 的意义在于让你对比不同算法（例如 `AoS` vs `SoA`）的耗时曲线。
3. **运行时间**：单元测试追求快，而高精度的 Benchmark 为了收集足够样本，往往需要运行较长时间，这不符合 `ctest` 快速反馈的初衷。
