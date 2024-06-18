#include <arm_neon.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>

#include "Timer.h"

// Matrix Dimensions
#define M 1024
#define N 512
#define K 2048

// Constants
#define TILE_SIZE 32
constexpr double GFLOP = (2.0 * M * N * K) * 1.0e-3;

// Matrices
float A[M * K];
float B[K * N];
float C[M * N];
float vals[M * N];

void gemm_naive() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                vals[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

void gemm_looporder() {
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < N; j++) {
                vals[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

void gemm_tiling() {
    for (int inner_tile = 0; inner_tile < K; inner_tile += TILE_SIZE) {
        for (int i = 0; i < M; i++) {
            int inner_tile_end = std::min(K, inner_tile + TILE_SIZE);
            for (int k = inner_tile; k < inner_tile_end; k++) {
                for (int j = 0; j < N; j++) {
                    vals[i * N + j] += A[i * K + k] * B[k * N + j];
                }
            }
        }
    }
}

void gemm_neon() {
    // TODO
}

void check_results() {
    for (int i = 0; i < M; i++) {
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
        std::print("Usage:\n\tgemm\n");
        exit(EXIT_FAILURE);
    }

    std::ifstream input("data.bin", std::ios::binary);

    if (input) {
        input.read(reinterpret_cast<char *>(&A), sizeof(float) * M * K);
        input.read(reinterpret_cast<char *>(&B), sizeof(float) * K * N);
        input.read(reinterpret_cast<char *>(&C), sizeof(float) * M * N);
        input.close();
    } else {
        std::print("[ERROR] Unable to locate data.bin, please use matmul.py to generate data file!\n");
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::print("Select Matrix Multiplication Method\n{:=>35}\n", '=');
    std::print("[1] Naive GEMM\n");
    std::print("[2] Loop order GEMM\n");
    std::print("[3] Tiling GEMM\n");
    std::print("[4] Neon GEMM\n > ");
    std::cin >> opt;

    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            gemm_naive();
            t.stop();
            break;
        }
        case 2: {
            t.start();
            gemm_looporder();
            t.stop();
            break;
        }
        case 3: {
            t.start();
            gemm_tiling();
            t.stop();
            break;
        }
        case 4: {
            t.start();
            gemm_neon();
            t.stop();
            break;
        }
        default:
            std::print("[ERROR] Invalid method selected!");
            exit(EXIT_FAILURE);
    }

    std::print("\nResults\n{:=>35}\n", '=');
    std::print("\tA Matrix [{} x {}]\n", M, K);
    std::print("\tB Matrix [{} x {}]\n", K, N);
    std::print("\tC Matrix [{} x {}]\n\n", M, N);
    std::print("\tM         = {:10}\n", M);
    std::print("\tN         = {:10}\n", N);
    std::print("\tK         = {:10}\n", K);
    std::print("\tGFLOPS    = {:10.5f}\n", GFLOP / (t.duration() * 1.0e3));
    std::print("\tTime (ms) = {:10}\n", t.duration());

    check_results();

    exit(EXIT_SUCCESS);
}