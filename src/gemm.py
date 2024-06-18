import numpy as np

m = 1024
n = 512
k = 2048

A = np.random.randn(m, k).astype(np.float32)
B = np.random.randn(k, n).astype(np.float32)
C = np.dot(A, B)

print(C)

with open("data.bin", "wb") as f:
    f.write(A.data)
    f.write(B.data)
    f.write(C.data)
