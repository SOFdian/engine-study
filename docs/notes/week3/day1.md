### Day 1：内存对齐与填充 (Alignment & Padding)

#### 今天学什么

- **内存对齐原理**：为什么 CPU 访问非对齐内存会损失性能。
- **编译器行为**：`sizeof` 的计算逻辑，`alignas` 与 `alignof` 的用法。
- **结构体重排**：如何通过调整成员顺序减少 Padding 空间。

#### 今天怎么学

1. 编写多个结构体，手动计算其 `sizeof` 预期值，再用 `std::alignment_of` 验证。
2. 实验：对比 `struct { char; int; char; }` 与 `struct { int; char; char; }` 的内存占用。
3. 学习使用 `static_assert` 检查结构体的大小和对齐。

#### 今天的产出物

- 《内存对齐与空间优化实验笔记》。
- `tests/test_alignment.cpp`。

============================================================================================================================

```
#include <iostream>
#include <cstddef> // offsetof 宏

struct StructA {
    char a;
    int b;
    char c;
};

struct StructB {
    int b;
    char a;
    char c;
};

int main() {
    // 验证大小
    static_assert(sizeof(StructA) == 12, "StructA size mismatch");
    static_assert(sizeof(StructB) == 8,  "StructB size mismatch");

    // 观察偏移量
    std::cout << "StructA - a: " << offsetof(StructA, a) 
              << ", b: " << offsetof(StructA, b) 
              << ", c: " << offsetof(StructA, c) << std::endl;
              
    return 0;
}
```

输出是：
```
StructA - a: 0, b: 4, c: 8
StructB - a: 4, b: 0, c: 5
```

============================================================================================================================

| **特性**     | **static_assert**                          | **assert**                                |
| ------------ | ------------------------------------------ | ----------------------------------------- |
| **生效阶段** | 编译期 (Compile-time)                      | 运行期 (Run-time)                         |
| **检查条件** | 必须是常量表达式 (Constant Expression)     | 可以是变量或运行时的状态                  |
| **性能开销** | **零开销**（编译完成后即消失）             | 有微小开销（通常在 Release 版中会被禁用） |
| **报错方式** | 编译器直接给出错误信息，无法生成可执行文件 | 程序运行到该行时终止并报错                |