#pragma once

#include <algorithm>

#if defined(__x86_64__)
#include <immintrin.h>
#endif

#define TILE_SIZE 4
#define BLOCK_SIZE 8  // 256 bits / 32 bits per float = 8 floats

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

#if defined(__x86_64__)
inline void AVX(const float *A, const float *B, float *C, int M, int N, int K) {
    // Zero initialize C
    for (int i = 0; i < M * N; i++) {
        C[i] = 0.0f;
    }

    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            __m256 a0 = _mm256_set1_ps(A[i * K + k]);  // Broadcast A[i, k] to all elements of a0
            for (int j = 0; j < N; j += BLOCK_SIZE) {
                // Load 8 elements from row k of B starting at column j
                __m256 b0 = _mm256_loadu_ps(B + k * N + j);

                // Load the current values of C[i, j:j+7]
                __m256 c0 = _mm256_loadu_ps(C + i * N + j);

                // Perform the fused multiply-add operation
                c0 = _mm256_fmadd_ps(a0, b0, c0);

                // Store the result back into C[i, j:j+7]
                _mm256_storeu_ps(C + i * N + j, c0);
            }
        }
    }
}
#endif
