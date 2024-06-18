#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <string>

#include "GEMM.h"
#include "Timer.h"

int get_arg_value(char *arg) {
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

    int M = get_arg_value(argv[2]);
    int N = get_arg_value(argv[4]);
    int K = get_arg_value(argv[6]);

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
    std::print("[4] Neon GEMM\n > ");
    std::cin >> opt;

    GEMM g;
    Timer t;
    switch (opt) {
        case 1: {
            t.start();
            g.naive(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 2: {
            t.start();
            g.looporder(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 3: {
            t.start();
            g.tiling(A, B, vals, M, N, K);
            t.stop();
            break;
        }
        case 4: {
            t.start();
            g.neon();
            t.stop();
            break;
        }
        default:
            std::print("[ERROR] Invalid method selected!");
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

    g.is_equal(C, vals, M, N);

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] vals;

    exit(EXIT_SUCCESS);
}