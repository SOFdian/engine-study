#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <core/Logger.h>

// 模拟一个简单的待测功能：假设我们要测试数学库（Day 11会深入）
int Addition(int a, int b) { return a + b; }

TEST_CASE("Testing basic engine utilities") {
    CHECK(Addition(1, 1) == 2);
    CHECK(Addition(-1, 1) == 0);
}

TEST_CASE("Logger Integrity Check") {
    // 验证 LOG 宏是否会引起崩溃
    LOG_INFO("Testing logger from unit test...");
    CHECK(true); 
}