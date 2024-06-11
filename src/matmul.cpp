#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>

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

int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::print("[USAGE]: ./matmul\n");
        exit(EXIT_FAILURE);
    }

    std::ifstream input("data.bin", std::ios::binary);

    if (input) {
        input.read(reinterpret_cast<char *>(&A), sizeof(float) * N * N);
        input.read(reinterpret_cast<char *>(&B), sizeof(float) * N * N);
        input.read(reinterpret_cast<char *>(&C), sizeof(float) * N * N);
        input.close();
    } else {
        std::print("[ERROR] Unable to locate data.bin, please use matmul.py to generate data file!\n");
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::print("Select Matrix Multiplication Method\n{:=>35}\n", '=');
    std::print("[1] Basic matmul\n");
    std::print("[2] Loop interchange matmul\n > ");
    std::cin >> opt;

    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            basic_matmul();
            t.stop();
            break;
        }
        case 2: {
            t.start();
            loop_interchange_matmul();
            t.stop();
            break;
        }
        default:
            std::print("[ERROR] Invalid method selected!");
            exit(EXIT_FAILURE);
    }

    std::print("\nResults\n{:=>35}\n", '=');
    std::print("\tN         = {:10}\n", N);
    std::print("\tGFLOPS    = {:10.4f}\n", GFLOP / (t.duration() * 1.0e3));
    std::print("\tTime (ms) = {:10}\n", t.duration());

    exit(EXIT_SUCCESS);
}