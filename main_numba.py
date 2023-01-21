from typing import List
import random
import time

import numba
from numba.types import ListType
import numpy as np


class Result:
    def __init__(self, start: int, stop: int, length: int) -> None:
        self.start = start
        self.stop = stop
        self.length = length

def find_longest_series(series: List[int]) -> Result:
    edges_current = [0, 0]
    edges_biggest = [0, 0]
    current_direction = 0
    prev_direction = 0
    for i in range(len(series)):
        if edges_current[1] - edges_current[0] > edges_biggest[1] - edges_biggest[0]:
            edges_biggest = edges_current.copy()
            
        if i == len(series) - 1:
            break

        if series[i+1] - series[i] > 0:
            current_direction = 1
        elif series[i+1] - series[i] < 0:
            current_direction = -1
        # no diff, still monotonic
        edges_current[1] = i+1
            
        if prev_direction != current_direction:
            edges_current[0] = i
        prev_direction = current_direction
    length = 0 if len(series) == 0 else edges_biggest[1] - edges_biggest[0] + 1
    res = Result(edges_biggest[0], edges_biggest[1], length)
    return res

@numba.njit('void(int32[::1])')
def find_longest_series_benchmark(series) -> None:
    edges_current = [0, 0]
    edges_biggest = [0, 0]
    # current_direction = 0
    # prev_direction = 0
    # for i in range(len(series)):
    #     if edges_current[1] - edges_current[0] > edges_biggest[1] - edges_biggest[0]:
    #         edges_biggest = edges_current.copy()
    #         
    #     if i == len(series) - 1:
    #         break
    #
    #     if series[i+1] - series[i] > 0:
    #         current_direction = 1
    #     elif series[i+1] - series[i] < 0:
    #         current_direction = -1
    #     # no diff, still monotonic
    #     edges_current[1] = i+1
    #         
    #     if prev_direction != current_direction:
    #         edges_current[0] = i
    #     prev_direction = current_direction
    # length = 0 if len(series) == 0 else edges_biggest[1] - edges_biggest[0] + 1
    # res = Result(edges_biggest[0], edges_biggest[1], length)


def benchmark():
    test = [random.randint(-100, 100) for _ in range(100)]
    test = np.array(test, dtype=np.int32)

    start = time.time()
    for _ in range(10000):
        test[0] = random.randint(-100, 100)
        find_longest_series_benchmark(test)

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
