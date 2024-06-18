#pragma once

#include <arm_neon.h>

#include <algorithm>
#include <print>

class GEMM {
   private:
    const int TILE_SIZE = 4;

   public:
    GEMM() = default;
    ~GEMM() = default;

    inline void naive(const float *A, const float *B, float *C, int M, int N, int K) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < K; k++) {
                    C[i * N + j] += A[i * K + k] * B[k * N + j];
                }
            }
        }
    }

    inline void looporder(const float *A, const float *B, float *C, int M, int N, int K) {
        for (int i = 0; i < M; i++) {
            for (int k = 0; k < K; k++) {
                for (int j = 0; j < N; j++) {
                    C[i * N + j] += A[i * K + k] * B[k * N + j];
                }
            }
        }
    }

    inline void tiling(const float *A, const float *B, float *C, int M, int N, int K) {
        for (int inner_tile = 0; inner_tile < K; inner_tile += TILE_SIZE) {
            for (int i = 0; i < M; i++) {
                int inner_tile_end = std::min(K, inner_tile + TILE_SIZE);
                for (int k = inner_tile; k < inner_tile_end; k++) {
                    for (int j = 0; j < N; j++) {
                        C[i * N + j] += A[i * K + k] * B[k * N + j];
                    }
                }
            }
        }
    }

    inline void neon() {
        // TODO
    }

    bool is_equal(const float *m1, const float *m2, int rows, int cols) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (fabsf(m1[i * rows + j] - m2[i * rows + j]) > 1.0e-3) {
                    std::print("\n[ERROR] Mismatch at ({0}, {1}) m1[{2}] != m2[{3}]\n", i, j, m1[i * rows + j], m2[i * cols + j]);
                    return false;
                }
            }
        }

        std::print("\n[SUCCESS] All done!\n");
        return true;
    }

    void print_matrix(const float *m, const int rows, const int cols) const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                std::print("{} ", m[i * rows + j]);
            }
            std::print("\n");
        }
        std::print("\n\n");
    }
};