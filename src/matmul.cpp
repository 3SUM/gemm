// g++ -Wall -pedantic -std=c++20 matmul.cpp -o matmul && ./matmul

#include <fstream>
#include <iomanip>
#include <iostream>

#define N 4

float A[N][N];
float B[N][N];
float C[N][N];

void print_matrix(float m[][N]) {
    std::cout << std::fixed << std::showpoint << std::setprecision(5);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc != 1) {
        std::cerr << "[ERROR] Usage: ./matmul" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input("data.bin", std::ios::binary);

    if(input.is_open()) {
        input.read(reinterpret_cast<char*>(&A), sizeof(float)*N*N);
        input.read(reinterpret_cast<char*>(&B), sizeof(float)*N*N);
        input.read(reinterpret_cast<char*>(&C), sizeof(float)*N*N);
    } else {
        std::cerr << "[ERROR] Unable to locate data.bin, please use data.py to generate data file!" << std::endl;
        exit(EXIT_FAILURE);
    }
    input.close();
    

    print_matrix(A);
    print_matrix(B);
    print_matrix(C);

    exit(EXIT_SUCCESS);
}