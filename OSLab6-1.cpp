#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int n = 2;
const int m = 3;
const int k = 2;

std::vector<std::vector<int>> A(n, std::vector<int>(m));
std::vector<std::vector<int>> B(m, std::vector<int>(k));
std::vector<std::vector<int>> C(n, std::vector<int>(k));

struct ThreadParam {
    int row;
    int col;
};

void* multiply(void* arg) {
    ThreadParam* param = (ThreadParam*)arg;
    int sum = 0;
    for (int i = 0; i < m; i++) {
        sum += A[param->row][i] * B[i][param->col];
    }
    C[param->row][param->col] = sum;
    std::cout << "[" << param->row << ", " << param->col << "] = " << sum << std::endl;
    delete param;
    return NULL;
}

void fillMatrices() {
    srand(time(NULL));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            A[i][j] = rand() % 10;
    
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < k; ++j)
            B[i][j] = rand() % 10;
}

int main() {
    fillMatrices();
    std::vector<pthread_t> threads(n * k);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            pthread_t tid;
            ThreadParam* param = new ThreadParam{i, j};
            pthread_create(&tid, NULL, multiply, param);
            threads[i * k + j] = tid;
        }
    }

    // Join all threads
    for (pthread_t& tid : threads) {
        pthread_join(tid, NULL);
    }

    return 0;
}