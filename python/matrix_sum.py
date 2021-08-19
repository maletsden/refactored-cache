import math
import datetime
import sys

MEASURE_ITER_NUM = 10
MATRIX_ROWS = 10_000
MATRIX_COLS = 10_000
EXPECTED_RESULT = MATRIX_ROWS * MATRIX_COLS

def isAlmostEqual(res):
    return math.isclose(res, EXPECTED_RESULT, rel_tol=1e-6)

def measureTime(data, func):
    minTime = sys.maxsize

    for i in range(MEASURE_ITER_NUM):
        start = datetime.datetime.now()
        res = func(data, MATRIX_ROWS, MATRIX_COLS);
        end = datetime.datetime.now()

        if not isAlmostEqual(res):
            raise Exception("ERROR: result is wrong.")

        time = (end - start).total_seconds() * 1e6
        minTime = min(minTime, time)

    return minTime

def printResult(solutionName, time):
    print(f'Solution with name "{solutionName}" has best time of {time} µs.')

def columnWiseRead(data, rows, cols):
    res = 0.0

    for row in range(rows):
        for col in range(cols):
            res += data[col * cols + row]
    return res


def rowWiseRead(data, rows, cols):
    res = 0.0;

    for row in range(rows):
        for col in range(cols):
            res += data[row * cols + col]
    return res


def unrollLoopRead(data, rows, cols):
    res = 0.0

    for row in range(rows):
        rowI = row * cols;
        for col in range(0, cols + 1, 5):
            res += data[rowI + col]
            res += data[rowI + col + 1]
            res += data[rowI + col + 2]
            res += data[rowI + col + 3]
            res += data[rowI + col + 4]
        for col in range(cols - (cols % 5), cols):
            res += data[rowI + col];

    return res

if __name__ == "__main__":
    data = [1.0] * (MATRIX_ROWS * MATRIX_COLS)

    printResult("column-wise-read", measureTime(data, columnWiseRead))
    printResult("row-wise-read", measureTime(data, rowWiseRead))