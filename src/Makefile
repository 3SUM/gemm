# Define a variable to control AVX compilation
AVX_ON ?= 0

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++2b -march=native -O3 -Wall -pedantic
AVXFLAGS = -mavx

# Source and output files
SRC = main.cpp
OUT = gemm

# If USE_AVX is set to 1, add AVXFLAGS to CXXFLAGS
ifeq ($(AVX_ON), 1)
    CXXFLAGS += $(AVXFLAGS)
endif

# Usage: make AVX_ON=1
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
