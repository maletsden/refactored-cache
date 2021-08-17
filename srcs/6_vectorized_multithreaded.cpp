//
// Created by maletsden on 17.08.21.
//

#include <vector>
#include <array>
#include <thread>
#include <numeric>
#include <immintrin.h>

#include "../hdrs/6_vectorized_multithreaded.h"

MATRIX_VAL vectorizedThreadedSum(const MATRIX_VAL* data, size_t rows, size_t cols) {

  constexpr auto threadsNum = 6;
  std::vector<std::thread> threads;
  threads.reserve(threadsNum);

  std::array<MATRIX_VAL, threadsNum * 64> results{};
  results.fill(MATRIX_VAL());

  for (auto threadI = 0; threadI < threadsNum; ++threadI) {
    const auto result = &results[threadI * 64];

    threads.emplace_back([threadI, rows, cols, data, result]() {
      const auto rowsPerThread = rows / threadsNum;
      const auto start = threadI * rowsPerThread;
      const auto end = (threadI + 1) == threadsNum ? rows : start + rowsPerThread;

      __m256d mmSum = _mm256_setzero_pd();
      constexpr auto loopStep = sizeof(decltype(mmSum)) / sizeof(MATRIX_VAL);

      for (auto row = start; row < end; ++row) {
        const auto rowI = row * cols;
        for (auto col = 0; col + loopStep <= cols; col += loopStep) {
          mmSum = _mm256_add_pd(mmSum, _mm256_loadu_pd(data + rowI + col));
        }

        for (auto col = cols - (cols % loopStep); col < cols; ++col) {
          *result += data[rowI + col];
        }
      }

      double vecRes[loopStep];
      _mm256_storeu_pd(vecRes, mmSum);

      for (auto vecRe : vecRes) {
        *result += vecRe;
      }
    });
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return std::accumulate(results.cbegin(), results.cend(), MATRIX_VAL());
}

MATRIX_VAL vectorizedUnrolledThreadedSum(const MATRIX_VAL* data, size_t rows, size_t cols) {

  constexpr auto threadsNum = 6;
  std::vector<std::thread> threads;
  threads.reserve(threadsNum);

  std::array<MATRIX_VAL, threadsNum * 64> results{};
  results.fill(MATRIX_VAL());

  for (auto threadI = 0; threadI < threadsNum; ++threadI) {
    const auto result = &results[threadI * 64];

    threads.emplace_back([threadI, rows, cols, data, result]() {
      const auto rowsPerThread = rows / threadsNum;
      const auto start = threadI * rowsPerThread;
      const auto end = (threadI + 1) == threadsNum ? rows : start + rowsPerThread;

      constexpr auto loopStep = sizeof(__m256d) / sizeof(MATRIX_VAL);
      constexpr auto unrollNum = 2;
      __m256d mmSum[unrollNum] = {_mm256_setzero_pd(), _mm256_setzero_pd()};

      for (auto row = start; row < end; ++row) {
        const auto rowI = row * cols;
        for (auto col = 0; col + loopStep * unrollNum <= cols; col += loopStep * unrollNum) {
          mmSum[0] = _mm256_add_pd(mmSum[0], _mm256_loadu_pd(data + rowI + col));
          mmSum[1] = _mm256_add_pd(mmSum[1], _mm256_loadu_pd(data + rowI + col + loopStep));
        }

        for (auto col = cols - (cols % (loopStep * unrollNum)); col < cols; ++col) {
          *result += data[rowI + col];
        }
      }

      double vecRes[loopStep];
      for (const auto &mm: mmSum) {
        _mm256_storeu_pd(vecRes, mm);

        for (auto vecRe : vecRes) {
          *result += vecRe;
        }
      }
    });
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return std::accumulate(results.cbegin(), results.cend(), MATRIX_VAL());
}

