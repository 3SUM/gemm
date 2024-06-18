import argparse
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument(
    "-m", dest="M", type=int, help="Number of rows for matrix A", required=True
)
parser.add_argument(
    "-n", dest="N", type=int, help="Number of columns for matrix B", required=True
)
parser.add_argument(
    "-k",
    dest="K",
    type=int,
    help="Number of columns/rows for matrices A & B",
    required=True,
)
args = parser.parse_args()

A = np.random.randn(args.M, args.K).astype(np.float32)
B = np.random.randn(args.K, args.N).astype(np.float32)
C = np.dot(A, B)

with open("data.bin", "wb") as f:
    f.write(A.data)
    f.write(B.data)
    f.write(C.data)
