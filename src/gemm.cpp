#include <arm_neon.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <string>

#include "Timer.h"

// Constants
#define TILE_SIZE 32

void gemm_naive(float *A, float *B, float *vals, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                vals[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

void gemm_looporder(float *A, float *B, float *vals, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < N; j++) {
                vals[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

void gemm_tiling(float *A, float *B, float *vals, int M, int N, int K) {
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

void check_results(float *C, float *vals, int M, int N) {
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

int get_value(char *arg) {
    int n = 0;
    std::istringstream ss(arg);
    if (!(ss >> n)) {
        std::print("[ERROR] Invalid value: {}", arg);
        exit(EXIT_FAILURE);
    } else if (!ss.eof()) {
        std::print("[ERROR] Trailing characters after number: {}", arg);
        exit(EXIT_FAILURE);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        std::print("Usage:\n\tgemm -m <M> -n <N> -k <K>\n\n");
        std::print("Options:\n");
        std::print("  -m M        Number of rows for matrix A\n");
        std::print("  -n N        Number of columns for matrix B\n");
        std::print("  -k K        Number of columns/rows for matrices A & B\n");
        exit(EXIT_FAILURE);
    }

    if (std::string(argv[1]) != "-m" || std::string(argv[3]) != "-n" || std::string(argv[5]) != "-k") {
        std::print("[ERROR] Invalid flags\n");
        exit(EXIT_FAILURE);
    }

    int M = get_value(argv[2]);
    int N = get_value(argv[4]);
    int K = get_value(argv[6]);

    float *A = new float[M * K];
    float *B = new float[K * N];
    float *C = new float[M * N];
    float *vals = new float[M * N];

    double GFLOP = (2.0 * M * N * K) * 1.0e-3;

    std::ifstream input("data.bin", std::ios::binary);

    if (input) {
        input.read(reinterpret_cast<char *>(A), sizeof(float) * M * K);
        input.read(reinterpret_cast<char *>(B), sizeof(float) * K * N);
        input.read(reinterpret_cast<char *>(C), sizeof(float) * M * N);
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
            gemm_naive(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 2: {
            t.start();
            gemm_looporder(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 3: {
            t.start();
            gemm_tiling(A, B, vals, M, N, K);
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

    check_results(C, vals, M, N);

    exit(EXIT_SUCCESS);
}