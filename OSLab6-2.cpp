#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

const int ITERATIONS = 1000000000;
int sharedVariable = 0;
std::mutex mtx;

void incrementWithoutSync() {
    for (int i = 0; i < ITERATIONS; ++i) {
        ++sharedVariable;
    }
}

void incrementWithSync() {
    for (int i = 0; i < ITERATIONS; ++i) {
        mtx.lock();
        ++sharedVariable;
        mtx.unlock();
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::thread t1(incrementWithoutSync);
    std::thread t2(incrementWithoutSync);
    t1.join();
    t2.join();
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Without synchronization: " << sharedVariable << ", Time: " << elapsed.count() << " s" << std::endl;

    sharedVariable = 0;

    start = std::chrono::high_resolution_clock::now();
    std::thread t3(incrementWithSync);
    std::thread t4(incrementWithSync);
    t3.join();
    t4.join();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "With synchronization: " << sharedVariable << ", Time: " << elapsed.count() << " s" << std::endl;

    return 0;
}