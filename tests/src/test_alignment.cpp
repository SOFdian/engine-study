#include <iostream>
#include <cstddef> // offsetof 宏

struct StructA
{
    char a;
    int b;
    char c;
};

struct StructB
{
    int b;
    char a;
    char c;
    double d;
};

int main()
{
    // 验证大小
    static_assert(sizeof(StructA) == 12, "StructA size mismatch");
    static_assert(sizeof(StructB) == 16, "StructB size mismatch");

    // 观察偏移量
    std::cout << "StructA - a: " << offsetof(StructA, a)
              << ", b: " << offsetof(StructA, b)
              << ", c: " << offsetof(StructA, c) << std::endl;
    std::cout << "StructB - a: " << offsetof(StructB, a)
              << ", b: " << offsetof(StructB, b)
              << ", c: " << offsetof(StructB, c)
              << ", d: " << offsetof(StructB, d)
              << std::endl;

    return 0;
}