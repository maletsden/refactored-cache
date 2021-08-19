//
// Created by maletsden on 17.08.21.
//
#include "../hdrs/5_multithreaded.h"

#include <vector>
#include <thread>
#include <numeric>
#include <mutex>

MATRIX_VAL threadedMutexReadSum(const MATRIX_VAL* data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();
  constexpr auto threadsNum = 6;

  std::vector<std::thread> threads;
  threads.reserve(threadsNum);

  std::mutex mx;
  for (auto threadI = 0; threadI < threadsNum; ++threadI) {
    threads.emplace_back([threadI, rows, cols, data, &res, &mx]() {
      const auto rowsPerThread = rows / threadsNum;
      const auto start = threadI * rowsPerThread;
      const auto end = (threadI + 1) == threadsNum ? rows : start + rowsPerThread;

      for (auto row = start; row < end; ++row) {
        for (auto col = 0; col < cols; ++col) {
            std::lock_guard<std::mutex> guard(mx);
            res += data[row * cols + col];
        }
      }

    });
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return res;
}

MATRIX_VAL threadedReadSum(const MATRIX_VAL* data, size_t rows, size_t cols) {

  constexpr auto threadsNum = 6;
  std::vector<std::thread> threads;
  threads.reserve(threadsNum);

  std::array<MATRIX_VAL, threadsNum> results{};
  results.fill(MATRIX_VAL());

  for (auto threadI = 0; threadI < threadsNum; ++threadI) {
    const auto result = &results[threadI];

    threads.emplace_back([threadI, rows, cols, data, result]() {
      const auto rowsPerThread = rows / threadsNum;
      const auto start = threadI * rowsPerThread;
      const auto end = (threadI + 1) == threadsNum ? rows : start + rowsPerThread;

      for (auto row = start; row < end; ++row) {
        const auto rowI = row * cols;
        for (auto col = 0; col < cols; ++col) {
          *result += data[rowI + col];
        }
      }
    });
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return std::accumulate(results.cbegin(), results.cend(), MATRIX_VAL());
}

MATRIX_VAL threadedCacheCoherencyReadSum(const MATRIX_VAL* data, size_t rows, size_t cols) {

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

      for (auto row = start; row < end; ++row) {
        const auto rowI = row * cols;
        for (auto col = 0; col < cols; ++col) {
          *result += data[rowI + col];
        }
      }
    });
  }

  for (auto& thread: threads) {
    thread.join();
  }

  return std::accumulate(results.cbegin(), results.cend(), MATRIX_VAL());
}