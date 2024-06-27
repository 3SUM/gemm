<br>

<p align="center">
<a href="https://github.com/3SUM"><img width="200" src="logo/gemm.png" alt="gemm logo"></a>
</p>

<br>

# GEMM

Various general matrix multiplcation (**GEMM**) methods implemented in C++.

If you have questions or concerns please feel free to contact me here or on **Discord @v0.3**

## GEMM Methods

Implementations of GEMM methods are located in [GEMM.h](src/GEMM.h).

### Naive

Basic nested-for loop approach.

### Loop interchange (Cache Aware)

Changing the order of the nested-for loops to provide more efficient memory access by preventing cache misses.

### Tiling (Cache Blocking)

Tiling is a technique used to optimize matrix multiplication by breaking down matrices into smaller, equal-sized subsets called tiles. These tiles are then stored in cache memory, which is faster than RAM.

## Build & Run

```
python3 gemm.py -m 1024 -n 1024 -k 1024
make
./gemm -m 1024 -n 1024 -k 1024
```
or
```
make AVX_ON=1
./gemm -m 1024 -n 1024 -k 1024
```
if you want to enable Advanced Vector Extensions (AVX)
```
Select Matrix Multiplication Method
===================================
[1] Naive GEMM
[2] Loop order GEMM
[3] Tiling GEMM
[4] Neon GEMM
[5] AVX (SIMD) GEMM
 > 2

Results
===================================
	A Matrix [1024 x 1024]
	B Matrix [1024 x 1024]
	C Matrix [1024 x 1024]

	M         =       1024
	N         =       1024
	K         =       1024
	GFLOPS    =   20.45223
	Time (ms) =        105

[SUCCESS] All done!
```

**Notes**:

-   **C++ std used is C++23, compiler version gcc/g++-14**.
-   [gemm.py](src/gemm.py) is used to generate a data file to populate matrices accordingly for [main](src/main.cpp) testing.
-   **Matrix dimensions must match for both programs**.

## License

&copy; [Luis Maya Aranda](https://github.com/3SUM). All rights reserved.

Licensed under the MIT License.
