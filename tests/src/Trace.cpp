#include <iostream>

class Trace {
public:
    int* data;
    size_t size;

    // 构造函数
    Trace(size_t n) : size(n) {
        data = new int[n];
        std::cout << "分配内存 🏗️" << std::endl;
    }

    // 1. 拷贝构造函数 (复制原件)
    Trace(const Trace& other) : size(other.size) {
        data = new int[size]; // 重新开辟一块地
        for (size_t i = 0; i < size; ++i) data[i] = other.data[i]; // 挨个搬运家具
        std::cout << "执行深拷贝 👥" << std::endl;
    }

    // 2. 移动构造函数 (接管原件)
    Trace(Trace&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr; // 关键：把原件的“钥匙”拿走，原件变空
        other.size = 0;
        std::cout << "执行移动操作 🚚" << std::endl;
    }

    ~Trace() {
        delete[] data; // 释放内存 🧹
    }
};