#include <fstream>
#include <iomanip>
#include <iostream>

#include "Timer.h"

#define N 1024

constexpr double GFLOP = (2.0 * N * N * N) * 1.0e-3;

float A[N * N];
float B[N * N];
float C[N * N];
float vals[N * N];

void basic_matmul() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                vals[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void loop_interchange_matmul() {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                vals[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void print_matrix(float m[]) {
    std::cout << std::fixed << std::showpoint << std::setprecision(4);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << m[i * N + j] << " ";
            if (N > 8 && j == 2) {
                j = N - 4;
                std::cout << "\t...\t";
            }
        }
        std::cout << std::endl;
        if (N > 8 && i == 2) {
            i = N - 4;
            std::cout << "\t\t\t\t...\n";
        }
    }

    std::cout << std::endl
              << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::cerr << "[USAGE]: ./matmul" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input("data.bin", std::ios::binary);

    if (input.is_open()) {
        input.read(reinterpret_cast<char *>(&A), sizeof(float) * N * N);
        input.read(reinterpret_cast<char *>(&B), sizeof(float) * N * N);
        input.read(reinterpret_cast<char *>(&C), sizeof(float) * N * N);
    } else {
        std::cerr << "[ERROR] Unable to locate data.bin, please use matmul.py to generate data file!" << std::endl;
        exit(EXIT_FAILURE);
    }
    input.close();

    int opt = 0;
    std::cout << "Select Matrix Multiplication Method\n-----------------------------------\n";
    std::cout << "[1] Basic matmul\n";
    std::cout << "[2] Loop interchange matmul\n";
    std::cout << "Enter option [digit]: ";
    std::cin >> opt;
    std::cout << std::endl;

    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            basic_matmul();
            t.end();
            std::cout << "basic_matmul(): " << GFLOP / (t.duration() * 1.0e3) << " GFLOPS ( " << t.duration() << " [ms] )" << std::endl;
            break;
        }
        case 2: {
            t.start();
            loop_interchange_matmul();
            t.end();
            std::cout << "loop_interchange_matmul(): " << GFLOP / (t.duration() * 1.0e3) << " GFLOPS ( " << t.duration() << " [ms] )" << std::endl;
            break;
        }
        default:
            std::cerr << "[ERROR] Invalid method selected!" << std::endl;
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}