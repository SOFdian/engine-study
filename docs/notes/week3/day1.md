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