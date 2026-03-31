### Day 4：万能引用与完美转发 (Perfect Forwarding)

#### 今天学什么

- **万能引用 (`T&&`)**：模板中的右值引用为何能同时接收左值和右值。
- **`std::forward`**：如何保持参数的原始属性。
- 模板实例化中的参数传递开销。

#### 今天怎么学

1. 编写一个简单的工厂函数模板 `MakeUnique<T>(Args&&... args)`。
2. 实验：如果不使用 `std::forward`，参数进入构造函数时会发生什么（属性退化）。
3. 阅读《Effective Modern C++》Item 23-25。



#### 今天的产出物

- 几个体现“完美转发”价值的示例代码（存入 `tests/`）。

=======================================================================================

```
#include <memory>
#include <utility> // 包含 std::forward

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
```

