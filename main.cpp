#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#include "hdrs/types.h"
#include "hdrs/timer.h"
#include "hdrs/1_colwise_read.h"
#include "hdrs/2_rowwise_read.h"
#include "hdrs/3_unroll_loop.h"
#include "hdrs/4_vectorized_loop.h"
#include "hdrs/5_multithreaded.h"
#include "hdrs/6_vectorized_multithreaded.h"

constexpr auto MEASURE_ITER_NUM = 10;
constexpr auto MATRIX_ROWS = 10'000;
constexpr auto MATRIX_COLS = 10'000;
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

void printResult(const std::string &solutionName, timer::TIME_TYPE time) {
  std::cout << "Solution with name \"" << solutionName << "\" has best time of " << time << " us." << std::endl;
}

int main() {
  const std::vector<MATRIX_VAL> data(MATRIX_ROWS * MATRIX_COLS, 1.);

  printResult("column-wise-read", measureTime(data.data(), colwiseReadSum));
  printResult("row-wise-read", measureTime(data.data(), rowwiseReadSum));
  printResult("unroll-loop-read", measureTime(data.data(), unrollLoopSum));
  printResult("sse-loop-read", measureTime(data.data(), vectorizedSSELoopSum));
  printResult("sse-unrolled-loop-read", measureTime(data.data(), vectorizedSSEUnrolledLoopSum));
  printResult("avx-loop-read", measureTime(data.data(), vectorizedAVXLoopSum));
  printResult("avx-unroll-loop-read", measureTime(data.data(), vectorizedAVXUnrollLoopSum));
  printResult("threaded-atomic-loop-read", measureTime(data.data(), threadedMutexReadSum));
  printResult("threaded-loop-read", measureTime(data.data(), threadedReadSum));
  printResult("threaded-cache-coherency-loop-read", measureTime(data.data(), threadedCacheCoherencyReadSum));
  printResult("vectorized-threaded-loop-read", measureTime(data.data(), vectorizedThreadedSum));
  printResult("vectorized-unrolled-threaded-loop-read", measureTime(data.data(), vectorizedUnrolledThreadedSum));

  return 0;
}
