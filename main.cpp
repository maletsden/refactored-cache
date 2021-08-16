#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#include "hdrs/types.h"
#include "hdrs/timer.h"
#include "hdrs/1_colwise_read.h"
#include "hdrs/2_rowwise_read.h"
#include "hdrs/3_unroll_loop.h"

constexpr auto MEASURE_ITER_NUM = 10;
constexpr auto MATRIX_ROWS = 10000;
constexpr auto MATRIX_COLS = 10000;
constexpr auto EXPECTED_RESULT = static_cast<MATRIX_VAL>(MATRIX_ROWS * MATRIX_COLS);

bool isAlmostEqual(MATRIX_VAL res) {
  return fabs(res - EXPECTED_RESULT) < std::numeric_limits<MATRIX_VAL>::epsilon();
}

auto
measureTime(const MATRIX_VAL *data, SUM_FUNC_PTR sum) {
  auto minTime = std::numeric_limits<timer::TIME_TYPE>::max();

  for (auto i = 0; i < MEASURE_ITER_NUM; ++i) {
    const auto start = timer::getCurrentTimeFenced();
    const auto res = sum(data, MATRIX_ROWS, MATRIX_COLS);
    const auto end = timer::getCurrentTimeFenced();

    if (!isAlmostEqual(res)) {
      throw std::runtime_error("ERROR: result is wrong.");
    }

    const auto time = timer::toUs(end - start);
    minTime = std::min<timer::TIME_TYPE>(minTime, time);
  }

  return minTime;
}

void printResult(const std::string& solutionName, timer::TIME_TYPE time) {
  std::cout << "Solution with name \"" << solutionName << "\" has best time of " << time << " us." << std::endl;
}

int main() {
  std::vector<MATRIX_VAL> data(MATRIX_ROWS * MATRIX_COLS, 1.);

  const auto time1 = measureTime(data.data(), colwiseReadSum);
  printResult("column-wise-read", time1);

  const auto time2 = measureTime(data.data(), rowwiseReadSum);
  printResult("row-wise-read", time2);

  const auto time3 = measureTime(data.data(), unrollLoopSum);
  printResult("unroll-loop-read", time3);

  return 0;
}
