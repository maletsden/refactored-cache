const microseconds = require('microseconds');

const MEASURE_ITER_NUM = 10;
const MATRIX_ROWS = 10_000;
const MATRIX_COLS = 10_000;
const EXPECTED_RESULT = MATRIX_ROWS * MATRIX_COLS;

function isAlmostEqual(res) {
    return Math.abs(res - EXPECTED_RESULT) < Number.EPSILON;
}

function measureTime(data, func) {
    let minTime = Number.MAX_SAFE_INTEGER;

    for (let i = 0; i < MEASURE_ITER_NUM; ++i) {
        const start = microseconds.now();
        const res = func(data, MATRIX_ROWS, MATRIX_COLS);
        const time  = microseconds.since(start);

        if (!isAlmostEqual(res)) {
            throw new Error("ERROR: result is wrong.");
        }

        minTime = Math.min(minTime, time);
    }

    return minTime;
}

function printResult(solutionName, time) {
    console.log(`Solution with name "${solutionName}" has best time of ${time} µs.`);
}

function columnWiseRead(data, rows, cols) {
    let res = 0.0;

    for (let row = 0; row < rows; ++row) {
        for (let col = 0; col < cols; ++col) {
            res += data[col * cols + row];
        }
    }

    return res;
}

function rowWiseRead(data, rows, cols) {
    let res = 0.0;

    for (let row = 0; row < rows; ++row) {
        const rowI = row * cols;
        for (let col = 0; col < cols; ++col) {
            res += data[rowI + col];
        }
    }

    return res;
}

function unrollLoopRead(data, rows, cols) {
    let res = 0.0;

    for (let row = 0; row < rows; ++row) {
        const rowI = row * cols;
        for (let col = 0; col + 5 <= cols; col += 5) {
            res += data[rowI + col];
            res += data[rowI + col + 1];
            res += data[rowI + col + 2];
            res += data[rowI + col + 3];
            res += data[rowI + col + 4];
        }

        for (let col = cols - (cols % 5); col < cols; ++col) {
            res += data[rowI + col];
        }
    }

    return res;

}

const data = new Float64Array(MATRIX_ROWS * MATRIX_COLS);
data.fill(1.0);

printResult("column-wise-read", measureTime(data, columnWiseRead));
printResult("column-wise-read", measureTime(data, rowWiseRead));
printResult("column-wise-read", measureTime(data, unrollLoopRead));
