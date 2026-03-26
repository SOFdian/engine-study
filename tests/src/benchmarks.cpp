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