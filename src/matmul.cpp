#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>

#include "Timer.h"

#define N 2048
#define TILE_SIZE 10

constexpr double GFLOP = (2.0 * N * N * N) * 1.0e-3;

float A[N * N];
float B[N * N];
float C[N * N];
float vals[N * N];

void matmul_naive() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                vals[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void matmul_looporder() {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            for (int j = 0; j < N; j++) {
                vals[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void matmul_tiling() {
    for (int inner_tile = 0; inner_tile < N; inner_tile += TILE_SIZE) {
        for (int i = 0; i < N; i++) {
            int inner_tile_end = std::min(N, inner_tile + TILE_SIZE);
            for (int k = inner_tile; k < inner_tile_end; k++) {
                for (int j = 0; j < N; j++) {
                    vals[i * N + j] += A[i * N + k] * B[k * N + j];
                }
            }
        }
    }
}

void check_results() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (fabsf(C[i * N + j] - vals[i * N + j]) > 1.0e-3) {
                std::print("\n[ERROR] Mismatch at ({0}, {1}) C[{2}] != vals[{3}]\n", i, j, C[i * N + j], vals[i * N + j]);
                exit(EXIT_FAILURE);
            }
        }
    }
    std::print("\n[SUCCESS] All done!\n");
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        std::print("Usage:\n\tmatmul\n");
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
    std::print("[1] Naive matmul\n");
    std::print("[2] Loop order matmul\n");
    std::print("[3] Tiling matmul\n > ");
    std::cin >> opt;

    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            matmul_naive();
            t.stop();
            break;
        }
        case 2: {
            t.start();
            matmul_looporder();
            t.stop();
            break;
        }
        case 3: {
            t.start();
            matmul_tiling();
            t.stop();
            break;
        }
        default:
            std::print("[ERROR] Invalid method selected!");
            exit(EXIT_FAILURE);
    }

    std::print("\nResults\n{:=>35}\n", '=');
    std::print("\tN         = {:10}\n", N);
    std::print("\tGFLOPS    = {:10.5f}\n", GFLOP / (t.duration() * 1.0e3));
    std::print("\tTime (ms) = {:10}\n", t.duration());

    check_results();

    exit(EXIT_SUCCESS);
}