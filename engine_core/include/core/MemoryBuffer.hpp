#include <iostream>
#include <algorithm> // 为了使用 std::swap

class MemoryBuffer {
public:
    // 1. 构造函数：获取资源
    explicit MemoryBuffer(size_t size) 
        : m_size(size), m_data(new float[size]()) { // () 初始化为零
        std::cout << "[Log] Allocated " << m_size << " floats." << std::endl;
    }

    // 2. 析构函数：释放资源 (RAII 的核心)
    ~MemoryBuffer() {
        if (m_data) {
            delete[] m_data;
            std::cout << "[Log] Freed memory." << std::endl;
        }
    }

    // 3. 移动构造函数 (Move Constructor)
    // 加上 noexcept 保证 std::vector 扩容时能使用它
    MemoryBuffer(MemoryBuffer&& other) noexcept 
        : m_data(nullptr), m_size(0) {
        // 直接利用 swap 窃取资源
        std::swap(m_data, other.m_data);
        std::swap(m_size, other.m_size);
    }

    // 4. 移动赋值运算符 (Move Assignment)
    MemoryBuffer& operator=(MemoryBuffer&& other) noexcept {
        if (this != &other) {
            // 先清空自己当前的资源（通过交换，让 other 帮我们释放）
            std::swap(m_data, other.m_data);
            std::swap(m_size, other.m_size);
        }
        return *this;
    }

    // 5. 禁用拷贝 (遵循本周“所有权唯一”原则)
    MemoryBuffer(const MemoryBuffer&) = delete;
    MemoryBuffer& operator=(const MemoryBuffer&) = delete;

    // 辅助方法
    size_t size() const { return m_size; }
    float* data() { return m_data; }

private:
    float* m_data = nullptr;
    size_t m_size = 0;
};