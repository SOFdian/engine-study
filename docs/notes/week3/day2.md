### Day 2：CPU 缓存体系结构 (Cache Locality)

#### 今天学什么

- **缓存层次**：L1、L2、L3 Cache 的基本量级与延迟。
- **Cache Line**：空间局部性（Spatial Locality）与时间局部性（Temporal Locality）。
- **预取机制**：CPU 如何猜测你下一步需要的数据。

#### 今天怎么学

1. 阅读关于“缓存友好型编程”的技术博客或《Game Engine Architecture》相关章节。
2. 实验：对比遍历 2D 数组时，“行优先”与“列优先”的性能巨大差异。
3. 使用 `ScopeTimer` 记录不同跳步（Stride）下内存访问的耗时曲线。

#### 今天的产出物

- 缓存行局部性验证代码。
- 绘制一份简单的内存访问步长与耗时的关联图表。

============================================================================================================================

## 一、 核心概念：CPU 的“三级跳”

CPU 处理数据的速度远快于内存（RAM）供货的速度。为了填补这个巨大的鸿沟，CPU 内部设计了三级缓存：

| **级别**         | **速度** | **容量**        | **延迟（参考值）** |
| ---------------- | -------- | --------------- | ------------------ |
| **L1 Cache**     | 极快     | 几十 KB         | ~4 周期            |
| **L2 Cache**     | 快       | 几百 KB 到几 MB | ~12 周期           |
| **L3 Cache**     | 一般     | 几十 MB         | ~40 周期           |
| **主内存 (RAM)** | 极慢     | 几 GB           | **~200+ 周期**     |

**关键结论**：一旦 CPU 在 L1/L2 中找不到数据（Cache Miss），它就必须停下来“发呆”，等待内存慢吞吞地把数据送过来。在引擎循环里，每秒钟发生数百万次这样的等待，性能就会彻底崩盘。

------

## 二、 空间局部性与 Cache Line

CPU 从内存抓取数据时，**不是按需抓取一个字节，而是顺便把周围的一片都抓过来**。这“一片”数据被称为 **Cache Line**，在你的 3060 Ti 配套的主流 CPU 上，它的大小通常是 **64 字节**。

- **空间局部性 (Spatial Locality)**：如果你访问了地址 $A$，那么你很有可能紧接着访问地址 $A+1$。
- **时间局部性 (Temporal Locality)**：如果你访问了地址 $A$，那么在不久的将来你很有可能再次访问它。

```

#include <iostream>
#include <vector>
#include <chrono>

const int SIZE = 10000;
int matrix[SIZE][SIZE]; // 注意：大型数组建议放在全局区或使用 std::vector 以防栈溢出

void test_row_major() {
    auto start = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Row-major: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    std::cout<<sum<<std::endl;
}

void test_col_major() {
    auto start = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (int j = 0; j < SIZE; ++j) { // 交换循环顺序
        for (int i = 0; i < SIZE; ++i) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Column-major: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    std::cout<<sum<<std::endl;
}

int main() {
    // 预热并初始化
    for(int i=0; i<SIZE; ++i) for(int j=0; j<SIZE; ++j) matrix[i][j] = 1;

    test_row_major();
    test_col_major();
    return 0;
}
```

