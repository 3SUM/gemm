#pragma once

#include <algorithm>

#define TILE_SIZE 4

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
