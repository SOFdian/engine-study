#include <core/Logger.h>
#include <core/Assert.h>

void ProcessPointer(int* ptr) {
    // 这里的逻辑假设：ptr 永远不应该是空指针
    CORE_ASSERT(ptr != nullptr, "Pointer must not be null!");
    
    LOG_INFO("Processing pointer...");
}

int main() {
    LOG_INFO("Testing Assert...");
    
    int* myPtr = nullptr;
    ProcessPointer(myPtr); // 这一行会触发断言失败
    
    return 0;
}