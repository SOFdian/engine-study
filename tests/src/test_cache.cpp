#include <iostream>
#include <vector>
#include <chrono>

const int SIZE = 10000;
int matrix[SIZE][SIZE]; // 注意：大型数组建议放在全局区或使用 std::vector 以防栈溢出

void test_row_major() {
    auto start = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Row-major: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    std::cout<<sum<<std::endl;
}

void test_col_major() {
    auto start = std::chrono::high_resolution_clock::now();
    long long sum = 0;
    for (int j = 0; j < SIZE; ++j) { // 交换循环顺序
        for (int i = 0; i < SIZE; ++i) {
            sum += matrix[i][j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Column-major: " << std::chrono::duration<double, std::milli>(end - start).count() << " ms\n";
    std::cout<<sum<<std::endl;
}

int main() {
    // 预热并初始化
    for(int i=0; i<SIZE; ++i) for(int j=0; j<SIZE; ++j) matrix[i][j] = 1;

    test_row_major();
    test_col_major();
    return 0;
}