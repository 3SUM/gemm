#pragma once

#include <cmath>
#include <print>
#include <sstream>

bool is_equal(const float *m1, const float *m2, const int rows, const int cols) {
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

void print_matrix(const float *m, const int rows, const int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::print("{} ", m[i * rows + j]);
        }
        std::print("\n");
    }
    std::print("\n\n");
}

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

    if (n <= 0) {
        std::print("[ERROR] Invalid value: {}", arg);
        exit(EXIT_FAILURE);
    }

    return n;
}
