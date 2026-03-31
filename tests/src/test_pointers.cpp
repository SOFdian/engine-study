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