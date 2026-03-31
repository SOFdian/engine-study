### Day 5：现代智能指针的边界 (Ownership)

#### 今天学什么

- **`std::unique_ptr`**：独占所有权与零开销。
- **`std::shared_ptr`**：引用计数、控制块（Control Block）与原子操作开销。
- **`std::weak_ptr`**：解决循环引用（重要：引擎场景树的基础）。

#### 今天怎么学

1. 模拟场景树：父节点用 `shared_ptr` 指向子节点，子节点用 `weak_ptr` 指向父节点。
2. **性能实验**：对比 100 万次 `unique_ptr` 解引用与 `shared_ptr` 拷贝的耗时。
3. 思考：为什么引擎内部高频使用的指针往往是裸指针（Raw Pointer）？

#### 今天的产出物

- `tests/test_pointers.cpp`。
- 笔记：智能指针的选择矩阵。

=======================================================================================

### 1. `std::unique_ptr`：默认的所有者 🥇

- **语义**：独占所有权。它是资源唯一的“主人”。
- **开销**：**零开销**。它的 `sizeof` 通常等于裸指针，且析构时自动释放资源。
- **原则**：在引擎中，除非你确定需要共享，否则请默认使用 `unique_ptr`。

### 2. `std::shared_ptr`：共享的所有者 👥

- **语义**：多个指针共同拥有一个对象。只有当最后一个 `shared_ptr` 销毁时，资源才会释放。
- **代价**：**非零开销**。它内部维护一个“控制块（Control Block）”，包含引用计数，且计数操作是**原子操作**（Atomic），在高频循环中会拖慢渲染帧率。

### 3. `std::weak_ptr`：旁观者（解决循环引用） 👁️

- **语义**：它“观测”对象，但不拥有它。它不增加引用计数。
- **必杀技**：打破“死亡环形引用”。

=======================================================================================

### std::unique_ptr

**内存布局**：它本质上就是一个裸指针的简单封装。在开启优化的情况下，它的汇编代码与裸指针完全一致。

**所有权控制**：它禁用了拷贝构造函数（`= delete`），但支持移动语义（`std::move`）。

**析构行为**：当 `unique_ptr` 离开作用域时，它会自动调用 `delete`。

=======================================================================================

### std::shared_ptr

### 底层原理：控制块（Control Block）

当你创建一个 `shared_ptr` 时，它不仅保存了指向对象的指针，还在堆上额外申请了一块内存，称为**控制块**：

- **引用计数（Strong Ref Count）**：记录有多少个 `shared_ptr` 指向该对象。
- **弱引用计数（Weak Ref Count）**：记录有多少个 `weak_ptr` 正在观察该对象。

=======================================================================================

### std::weak_ptr

`weak_ptr` 是为了解决 `shared_ptr` 的致命缺陷——**循环引用**而生的。

### 它的特殊之处

- **不拥有所有权**：它不会增加控制块中的“强引用计数”。
- **不能直接访问**：你不能直接用 `weak_ptr` 去调用对象的方法。你必须先调用 `.lock()`，如果对象还活着，它会返回一个 `shared_ptr`，否则返回空。

```
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <string>

// 辅助类：用于追踪对象的生命周期
class TraceNode {
public:
    explicit TraceNode(std::string name) : m_name(std::move(name)) {
        std::cout << "[Create] Node: " << m_name << std::endl;
    }
    ~TraceNode() {
        std::cout << "[Destroy] Node: " << m_name << std::endl;
    }

    void SetChild(std::shared_ptr<TraceNode> child) { m_child = std::move(child); }
    
    // 关键点：使用 weak_ptr 指向父节点以打破循环引用
    void SetParent(std::shared_ptr<TraceNode> parent) { m_parent = parent; }

    std::string GetName() const { return m_name; }

private:
    std::string m_name;
    std::shared_ptr<TraceNode> m_child;      // 父拥有的子：强引用
    std::weak_ptr<TraceNode> m_parent;      // 子引用的父：弱引用 (关键！)
};

// --- 实验 1: unique_ptr 的所有权转移 ---
void TestUniquePtr() {
    std::cout << "\n>>> Running TestUniquePtr..." << std::endl;
    auto nodeA = std::make_unique<TraceNode>("Unique_A");
    
    // auto nodeB = nodeA; // 编译错误！unique_ptr 不允许拷贝
    auto nodeB = std::move(nodeA); // 显式所有权转移

    if (!nodeA) {
        std::cout << "nodeA is now null (Ownership moved to nodeB)." << std::endl;
    }
} // nodeB 离开作用域，Unique_A 自动销毁

// --- 实验 2: shared_ptr 与 weak_ptr 的循环引用突破 ---
void TestSceneTree() {
    std::cout << "\n>>> Running TestSceneTree (Weak Pointers)..." << std::endl;
    {
        auto parent = std::make_shared<TraceNode>("ParentNode");
        auto child = std::make_shared<TraceNode>("ChildNode");

        parent->SetChild(child);   // Parent -> Child (Shared)
        child->SetParent(parent);  // Child -> Parent (Weak)

        std::cout << "Parent count: " << parent.use_count() << std::endl; // 应为 1
        std::cout << "Child count: " << child.use_count() << std::endl;   // 应为 2 (parent 拥有它)
    } 
    // 作用域结束：由于 weak_ptr 不增加计数，Parent 计数归零触发销毁，
    // 进而导致 Child 的计数归零并销毁。如果没有 weak_ptr，此处将发生内存泄漏。
}

// --- 实验 3: 性能对比 (100 万次操作) ---
void RunPerformanceBenchmark() {
    std::cout << "\n>>> Running Performance Benchmark (1M iterations)..." << std::endl;
    const int iterations = 1000000;

    // A. unique_ptr 解引用开销
    auto uPtr = std::make_unique<int>(42);
    auto startU = std::chrono::high_resolution_clock::now();
    volatile int sumU = 0;
    for (int i = 0; i < iterations; ++i) {
        sumU += *uPtr; // O(1) 解引用
    }
    auto endU = std::chrono::high_resolution_clock::now();

    // B. shared_ptr 拷贝开销 (涉及原子操作计数)
    auto sPtr = std::make_shared<int>(42);
    auto startS = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        std::shared_ptr<int> copy = sPtr; // 原子操作增加计数
    }
    auto endS = std::chrono::high_resolution_clock::now();

    auto durU = std::chrono::duration_cast<std::chrono::milliseconds>(endU - startU).count();
    auto durS = std::chrono::duration_cast<std::chrono::milliseconds>(endS - startS).count();

    std::cout << "unique_ptr dereference time: " << durU << " ms" << std::endl;
    std::cout << "shared_ptr copy (atomic) time: " << durS << " ms" << std::endl;
}

int main() {
    TestUniquePtr();
    TestSceneTree();
    RunPerformanceBenchmark();
    
    std::cout << "\n[All Tests Finished]" << std::endl;
    return 0;
}
```

