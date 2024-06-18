import numpy as np

M = 16
N = 32
K = 64

A = np.random.randn(M, K).astype(np.float32)
B = np.random.randn(K, N).astype(np.float32)
C = np.dot(A, B)

with open("data.bin", "wb") as f:
    f.write(A.data)
    f.write(B.data)
    f.write(C.data)
