#include <memory>
#include <utility> // 包含 std::forward
#include "core/MemoryBuffer.hpp"

// 1. typename... Args: 变长参数模板，代表可以接收任意数量、任意类型的参数
template<typename T, typename... Args>
std::unique_ptr<T> MyMakeUnique(Args&&... args) {
    
    // 2. Args&&... args: 这里的 && 是万能引用
    // 它能同时捕获左值（如变量）和右值（如临时常量）
    
    // 3. std::forward<Args>(args)...: 完美转发
    // 它的作用是：如果传入的是右值，就继续以右值身份传给构造函数（触发移动语义）
    // 如果传入的是左值，就以左值身份传递（触发拷贝语义）
    
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


int main(){
    auto ptr = MyMakeUnique<MemoryBuffer>(MemoryBuffer(1024));
    return 0;
}