### Day 2：RAII 与资源管理封装

#### 今天学什么

- **RAII 核心思想**：资源获取即初始化。
- **Rule of Five**：三/五/零法则。
- 显式删除（`= delete`）拷贝操作的意义。

#### 今天怎么学

1. 实现一个 `FileHandle` 类封装 `FILE*`。
2. 实现一个 `MemoryBuffer` 类，在构造时 `new` 内存，析构时 `delete`。
3. 练习：如何让一个类“只能移动，不能拷贝”。

#### 今天的产出物

- `FileHandle.hpp` (集成到 `engine_core/include`)。
- `MemoryBuffer.hpp`。

=======================================================================================

## RAII 核心思想(Resource Acquisition Is Initialization)

**构造时获取**：在对象构造函数中分配资源（如打开文件、申请内存）。

**析构时释放**：在对象析构函数中自动释放资源。 这意味着只要对象离开作用域（无论是因为 `return`、函数结束还是抛出异常），资源都会被确定性地回收。

核心是：**将资源的生命周期与局部对象（栈对象）的生命周期强绑定**。

栈对象拥有**确定性的析构**特性：

- **正常退出**：函数执行完毕，栈帧销毁，局部对象析构。
- **异常退出**：如果代码抛出错误，C++ 会进行“栈回退”（Stack Unwinding），同样会依次析构已构造的局部对象。

=======================================================================================

## 实现 FileHandle.hpp

```
#include <cstdio>
#include <iostream>

class FileHandle
{
public:
    // 构造函数：获取资源
    explicit FileHandle(const char *filename, const char *mode)
    {
        m_handle = std::fopen(filename, mode);
        if (!m_handle)
        {
            std::cerr << "Failed to open file!" << std::endl;
        }
    }
    
    // 析构函数：释放资源
    ~FileHandle()
    {
        if (m_handle != nullptr)
        {
            std::fclose(m_handle); 
            m_handle = nullptr;    
        }
    }
    // 提供一个只读访问句柄的方法
    FILE *get() const { return m_handle; }

private:
    FILE *m_handle = nullptr;
};
```

- explicit：**防止编译器进行隐式的类型转换**

  **禁止的行为**：`MemoryBuffer buf = 10;`（编译报错）

  **允许的行为**：`MemoryBuffer buf(10);` 或 `MemoryBuffer buf = MemoryBuffer(10);`

```
    // 禁止拷贝：防止资源所有权混乱
    // 例如FileHandle fileB(fileA);
    FileHandle(const FileHandle &) = delete;

    // TODO: 使用 = delete 禁用拷贝赋值运算符
    // 例如FileHandle fileB = fileA;
    FileHandle &operator=(const FileHandle &) = delete;
```

- 拷贝构造函数：`FileHandle(const FileHandle&)`，当一个**全新的对象**正在被创建，并且它的初始状态需要从一个已存在的同类对象复制而来时，会调用此函数。`FileHandle fileB(fileA);` 或 `FileHandle fileB = fileA;`。
- 拷贝赋值运算符：`FileHandle& operator=(const FileHandle&)`，当一个**已经存在（已经构造完成）的对象**，需要丢弃它原本拥有的资源，转而接纳另一个对象的内容时，会调用此函数。

```
    FileHandle(FileHandle &&other) noexcept
    {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
    }

    FileHandle &operator=(FileHandle &&other) noexcept
    {
        // 自赋值检查
        if (this != &other)
        {
            // A. 释放自己当前的资源（RAII 要求）
            if (m_handle)
            {
                std::fclose(m_handle);
            }

            // B. 窃取对方资源
            m_handle = other.m_handle;

            // C. 将对方置于安全状态
            other.m_handle = nullptr;
        }
        return *this;
    }
```

- noexcept ：作为**说明符**（声明函数不会抛出异常）和作为**运算符**（检查表达式是否会抛出异常），如果一个声明为`noexcept`的函数抛出了异常：
  - 异常**不会**被传播到函数外部
  - 调用`std::terminate()`终止程序
  - 不保证会执行[栈展开](https://zhida.zhihu.com/search?content_id=257946283&content_type=Article&match_order=1&q=栈展开&zhida_source=entity)（不会调用局部对象的析构函数）

​	如果移动构造函数**没有**标注 `noexcept`，编译器会认为移动过程可能会抛出异常（例如移动到一半内存不足）。为了保证原数据不损坏，`std::vector` 会退而求其次，选择使用**拷贝构造**（$O(n)$）而不是移动。

```
    // 也可以这样写
    // FileHandle &operator=(FileHandle &&other) noexcept
    // {
    //     if (this != &other)
    //     {
    //         // 通过交换，让 other 替我们去析构旧资源
    //         // 当 other 离开作用域时，它原本持有的（现在在 swap 后变成我们的旧资源）会被自动释放
    //         std::swap(m_handle, other.m_handle);
    //     }
    //     return *this;
    // }
```

- `std::swap` 的内部运作与逻辑简化：

```
template <typename T>
void swap(T& a, T& b) noexcept {
    T temp = std::move(a); // 1. a 的所有权转移给 temp
    a = std::move(b);      // 2. b 的所有权转移给 a
    b = std::move(temp);   // 3. temp 的所有权转移给 b
}
```

