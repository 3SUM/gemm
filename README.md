<br>

<p align="center">
<a href="https://github.com/3SUM"><img width="200" src="./logo/gemm.png" alt="gemm logo"></a>
</p>

<br>

# GEMM

Various general matrix multiplcation (**GEMM**) methods implemented in C++.

If you have questions or concerns please feel free to contact me here or on **Discord @im.upset**.

## GEMM Methods

### Naive

Basic nested-for loop approach.

### Loop interchange (Cache Aware)

Changing the order of the nested-for loops to provide more efficient memory access by preventing cache misses.

### Tiling (Cache Blocking)

Tiling is a technique used to optimize matrix multiplication by breaking down matrices into smaller, equal-sized subsets called tiles. These tiles are then stored in cache memory, which is faster than RAM.

## Build & Run

```
git clone https://github.com/3SUM/gemm.git
cd gemm/src
python3 gemm.py -m <A_ROWS> -n <B_COLS> -k <A_COLS/B_ROWS>
make
./gemm -m <A_ROWS> -n <B_COLS> -k <A_COLS/B_ROWS>
```

**Notes**:

-   [gemm.py](src/gemm.py) is used to generate a data file to populate matrices accordingly for [gemm.cpp](src/gemm.cpp).
-   **Matrix dimensions must match for both programs**

## Example Usage & Output

```
python3 gemm.py -m 1024 -n 1024 -k 1024
make
./gemm -m 1024 -n 1024 -k 1024

Select Matrix Multiplication Method
===================================
[1] Naive GEMM
[2] Loop order GEMM
[3] Tiling GEMM
[4] Neon GEMM
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

## License

&copy; [Luis Maya Aranda](https://github.com/3SUM). All rights reserved.

Licensed under the MIT License.
