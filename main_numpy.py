from typing import List
import random
import time

import numpy as np


class Result:
    def __init__(self, start: int, stop: int, length: int) -> None:
        self.start = start
        self.stop = stop
        self.length = length

def find_longest_series(series: List[int]) -> Result:
    if len(series) < 1:
        return Result(0, 0, 0)

    # @evsikov_a
    idxs = [i for i in range(len(series))]

    split_idxs = np.where(np.abs(np.pad(np.diff(np.sign(np.diff(series))),1)) == 2)[0]
# split_idxs = np.where(np.pad(np.diff(np.sign(np.diff(a))),1) < 0)[0]
    splitted_idxs = np.split(idxs, split_idxs)

    idxs_of_max_monotonic_seq = splitted_idxs[0]
    max_monotonic_len = 0
    for i, chunk_idxs in enumerate(splitted_idxs):
        if i != len(splitted_idxs):
            chunk_idxs = np.hstack((chunk_idxs, chunk_idxs[-1]+1))
        chunk_len = len(chunk_idxs)
        if chunk_len > max_monotonic_len:
            max_monotonic_len = chunk_len
            idxs_of_max_monotonic_seq = chunk_idxs

    res = Result(idxs_of_max_monotonic_seq[0], idxs_of_max_monotonic_seq[-1], len(idxs_of_max_monotonic_seq))
    return res;


def benchmark():
    test = [random.randint(-100, 100) for _ in range(100)]

    start = time.time()
    for _ in range(10000):
        test[0] = random.randint(-100, 100)
        find_longest_series(test)

    end = time.time()
    print(f"Elapsed time in microseconds: {int((end - start) * 1_000_000)} µs")

def main():
    tests = [
        [5, 15, 14, 13, 12, 16, 10],
        [5],
        [],
        [1,2,3],
        [3,2,1],
        [3,2,1,2,3],
        [1,2,1],
        [1,2,2,2,1],
        [5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5],
        [5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5],
        [5, 5, 5, 5, 5, 16, 10, 5, 5, 5, 5, 5, 5],
        [5, 5, 5, 5, 5, 16, 10],
        [5, -15, -14, -13, -12, 16, 10],
        [5, -12, -13, -14, -15, 16, 10],
        [5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20],
        [5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 11, 10, 15, 14, 13, 12, 11, 10, 9, -10, -20, 2, 3],
        [5, 15, 14, 13, 12, 16, 10, 15, 14, 13, 12, 13, 12, 11, 10, 9, -10, -20, 11, 10, 15, 14]
    ]
    print("Challenge accepted!")
    for test in tests:
        print(",".join(map(lambda x: str(x), test)))
        res = find_longest_series(test)
        print(f"res [{res.start}, {res.stop}]  len: {res.length}")

    for _ in range(10):
        benchmark()

if __name__ == "__main__":
    main()
