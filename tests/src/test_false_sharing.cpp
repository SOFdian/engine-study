#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <new> // 用于 std::hardware_destructive_interference_size

// 迭代次数，设大一点以便观察差异
const long long ITERATIONS = 100'000'0000;

// 情况 A：存在伪共享 (False Sharing)
struct BadCounter {
    // a 和 b 紧挨着，极大概率在同一个 64 字节的 Cache Line 里
    volatile long long a = 0;
    volatile long long b = 0;
};

// 情况 B：消除伪共享 (通过 alignas 隔离)
// 告诉编译器：在这个结构体定义期间，不要报 C4324 警告
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

struct GoodCounter {
    alignas(64) volatile long long a = 0; // 强制隔离到不同 Cache Line
    alignas(64) volatile long long b = 0;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

// 累加任务函数
void work(volatile long long& counter) {
    for (long long i = 0; i < ITERATIONS; ++i) {
        counter++;
    }
}

template <typename T>
void run_benchmark(const std::string& label) {
    T counters;
    
    auto start = std::chrono::high_resolution_clock::now();

    // 开启两个线程，分别修改不同的变量
    std::thread t1(work, std::ref(counters.a));
    std::thread t2(work, std::ref(counters.b));

    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << label << " took: " << duration << " ms" << std::endl;
}

int main() {
    std::cout << "--- Starting False Sharing Benchmark ---" << std::endl;
    
    // 运行存在伪共享的测试
    run_benchmark<BadCounter>("Bad (False Sharing)");
    
    // 运行优化后的测试
    run_benchmark<GoodCounter>("Good (Aligned/Isolated)");

    return 0;
}