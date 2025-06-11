AVX_ON ?= 0

CXX = g++
CXXFLAGS = -std=c++2b -march=native -O3 -Wall -pedantic
AVXFLAGS = -mavx

SRC = main.cpp
OUT = gemm

ifeq ($(AVX_ON), 1)
    CXXFLAGS += $(AVXFLAGS)
endif

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
