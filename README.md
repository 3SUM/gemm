<br>

<p align="center">
<a href="https://github.com/3SUM"><img width="200" src="./logo/matmul.png" alt="matmul logo"></a>
</p>

<br>

# matmul

Various matrix multiplcation methods implemented in C++.

If you have questions or concerns please feel free to contact me here or on **Discord @im.upset**.

## Matrix Multiplication Methods

#### Naive

Basic nested-for loop approach.

#### Loop interchange (Cache Aware)

Changing the order of the nested-for loops to provide more efficient memory access by preventing cache misses.

#### Tiling (Cache Blocking)

Tiling is a technique used to optimize matrix multiplication by breaking down matrices into smaller, equal-sized subsets called tiles. These tiles are then stored in cache memory, which is faster than RAM.

## Build & Run

```
git clone https://github.com/3SUM/matmul.git
cd matmul/src
python3 matmul.py
make
./matmul
```

**Note**: [matmul.py](src/matmul.py) is used to generate a data file to populate matrices accordingly for testing.

## License

&copy; [Luis Maya Aranda](https://github.com/3SUM). All rights reserved.

Licensed under the MIT License.
