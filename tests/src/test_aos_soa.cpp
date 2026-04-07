#include <iostream>
#include <vector>
#include <chrono>
#include <random>

// 1. 定义实验规模：100 万个粒子
const int N = 1000000; 

// --- AoS 风格 ---
struct ParticleAoS {
    float x, y, z;
    float vx, vy, vz;
    uint32_t color;
};

// --- SoA 风格 ---
struct ParticleSoA {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<uint32_t> color;

    ParticleSoA(int n) : x(n), y(n), z(n), vx(n), vy(n), vz(n), color(n) {}
};

// 模拟“全属性更新”逻辑
void update_aos(std::vector<ParticleAoS>& particles) {
    for (auto& p : particles) {
        p.x += p.vx;
        p.y += p.vy;
        p.z += p.vz;
        p.color = 0xFFFFFFFF; // 模拟修改所有属性
    }
}

void update_soa(ParticleSoA& p) {
    for (int i = 0; i < N; ++i) {
        p.x[i] += p.vx[i];
        p.y[i] += p.vy[i];
        p.z[i] += p.vz[i];
        p.color[i] = 0xFFFFFFFF; // 模拟修改所有属性
    }
}

int main() {
    // 初始化数据
    std::vector<ParticleAoS> aos_data(N, {0,0,0, 1,1,1, 0});
    ParticleSoA soa_data(N);
    for(int i=0; i<N; ++i) {
        soa_data.vx[i] = soa_data.vy[i] = soa_data.vz[i] = 1.0f;
    }

    // --- 测试 AoS ---
    auto start = std::chrono::high_resolution_clock::now();
    update_aos(aos_data);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "AoS Total Update: " 
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    // --- 测试 SoA ---
    start = std::chrono::high_resolution_clock::now();
    update_soa(soa_data);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SoA Total Update: " 
              << std::chrono::duration<double, std::milli>(end - start).count() << " ms" << std::endl;

    return 0;
}