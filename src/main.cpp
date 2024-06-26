#include <fstream>
#include <iostream>
#include <print>
#include <string>

#include "GEMM.h"
#if defined(__ARM_NEON__)
#include "NeonGEMM.h"
#endif
#include "Timer.h"
#include "Utils.h"

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

    int M = get_arg_value(argv[2]); // rows of matrix A
    int N = get_arg_value(argv[4]); // columns of matrix B
    int K = get_arg_value(argv[6]); // number of columns/rows A&B

    float *A = nullptr;
    float *B = nullptr;
    float *C = nullptr;
    float *vals = nullptr;

    double GFLOP = (2.0 * M * N * K) * 1.0e-3;

    std::ifstream input("data.bin", std::ios::binary);

    if (input) {
        A = new float[M * K];
        B = new float[K * N];
        C = new float[M * N];
        vals = new float[M * N];

        input.read(reinterpret_cast<char *>(A), sizeof(float) * M * K);
        input.read(reinterpret_cast<char *>(B), sizeof(float) * K * N);
        input.read(reinterpret_cast<char *>(C), sizeof(float) * M * N);
        input.close();
    } else {
        std::print("[ERROR] Unable to locate data.bin, please use gemm.py to generate data file!\n");
        exit(EXIT_FAILURE);
    }

    int opt = 0;
    std::print("Select Matrix Multiplication Method\n{:=>35}\n", '=');
    std::print("[1] Naive GEMM\n");
    std::print("[2] Loop order GEMM\n");
    std::print("[3] Tiling GEMM\n");
    std::print("[4] Neon GEMM\n");
    std::print("[5] AVX (SIMP) GEMM\n>");
    std::cin >> opt;

    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            naive(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 2: {
            t.start();
            looporder(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 3: {
            t.start();
            tiling(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 4: {
            t.start();
#if defined(__ARM_NEON__)
            neon(A, B, vals, M, N, K);
#else
            std::print("[ERROR] ARM Neon not supported on this architecture!\n");
#endif
            t.stop();
            break;
        }
        case 5:
            t.start();
#if defined(__x86_64__)
            AVX(A, B, vals, M, N, K);
#else
            std::print("[ERROR] x86 AVX not supported on this architecture!\n");
#endif
            t.stop();
            break;
        default:
            std::print("[ERROR] Invalid method selected!\n");
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

    is_equal(C, vals, M, N);

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] vals;

    exit(EXIT_SUCCESS);
}
