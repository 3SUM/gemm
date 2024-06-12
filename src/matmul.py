import numpy as np

N = 2048

A = np.random.randn(N, N).astype(np.float32)
B = np.random.randn(N, N).astype(np.float32)
C = np.dot(A, B)

with open("data.bin", "wb") as f:
    f.write(A.data)
    f.write(B.data)
    f.write(C.data)
