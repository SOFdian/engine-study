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
        // TODO: 实现资源释放逻辑
        if (m_handle)
        {
            std::fclose(m_handle);
        }
    }

    // 禁止拷贝：防止资源所有权混乱
    // TODO: 使用 = delete 禁用拷贝构造函数
    // 例如FileHandle fileB(fileA);
    FileHandle(const FileHandle &) = delete;

    // TODO: 使用 = delete 禁用拷贝赋值运算符
    // 例如FileHandle fileB = fileA;
    FileHandle &operator=(const FileHandle &) = delete;

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

    // 提供一个只读访问句柄的方法
    FILE *get() const { return m_handle; }

private:
    FILE *m_handle = nullptr;
};