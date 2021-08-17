//
// Created by maletsden on 17.08.21.
//

#include <immintrin.h>
#include <xmmintrin.h>

#include "../hdrs/4_vectorized_loop.h"

MATRIX_VAL vectorizedSSELoopSum(const MATRIX_VAL *data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  constexpr auto loopStep = sizeof(__m128d) / sizeof(MATRIX_VAL);
  __m128d mmSum = _mm_setzero_pd();

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col + loopStep <= cols; col += loopStep) {
      mmSum = _mm_add_pd(mmSum, _mm_loadu_pd(data + rowI + col));
    }

    for (auto col = cols - (cols % loopStep); col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  double vecRes[loopStep];
  _mm_storeu_pd(vecRes, mmSum);
  for (auto vecRe : vecRes) {
    res += vecRe;
  }

  return res;
}

MATRIX_VAL vectorizedSSEUnrolledLoopSum(const MATRIX_VAL *data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  constexpr auto loopStep = sizeof(__m128d) / sizeof(MATRIX_VAL);
  constexpr auto unrollNum = 2;
  __m128d mmSum[unrollNum] = {_mm_setzero_pd(), _mm_setzero_pd()};

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col + loopStep * unrollNum <= cols; col += loopStep * unrollNum) {
      mmSum[0] = _mm_add_pd(mmSum[0], _mm_loadu_pd(data + rowI + col));
      mmSum[1] = _mm_add_pd(mmSum[1], _mm_loadu_pd(data + rowI + col + loopStep));
    }

    for (auto col = cols - (cols % (loopStep * unrollNum)); col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  double vecRes[loopStep];

  for (const auto &mm: mmSum) {
    _mm_storeu_pd(vecRes, mm);

    for (auto vecRe : vecRes) {
      res += vecRe;
    }
  }

  return res;
}

MATRIX_VAL vectorizedAVXLoopSum(const MATRIX_VAL *data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  __m256d mmSum = _mm256_setzero_pd();
  constexpr auto loopStep = sizeof(decltype(mmSum)) / sizeof(MATRIX_VAL);

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col + loopStep <= cols; col += loopStep) {
      mmSum = _mm256_add_pd(mmSum, _mm256_loadu_pd(data + rowI + col));
    }

    for (auto col = cols - (cols % loopStep); col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  double vecRes[loopStep];
  _mm256_storeu_pd(vecRes, mmSum);

  for (auto vecRe : vecRes) {
    res += vecRe;
  }

  return res;
}

MATRIX_VAL vectorizedAVXUnrollLoopSum(const MATRIX_VAL *data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  constexpr auto loopStep = sizeof(__m256d) / sizeof(MATRIX_VAL);
  constexpr auto unrollNum = 2;
  __m256d mmSum[unrollNum] = {_mm256_setzero_pd(), _mm256_setzero_pd()};

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col + loopStep * unrollNum <= cols; col += loopStep * unrollNum) {
      mmSum[0] = _mm256_add_pd(mmSum[0], _mm256_loadu_pd(data + rowI + col));
      mmSum[1] = _mm256_add_pd(mmSum[1], _mm256_loadu_pd(data + rowI + col + loopStep));
    }

    for (auto col = cols - (cols % (loopStep * unrollNum)); col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  double vecRes[loopStep];

  for (const auto &mm: mmSum) {
    _mm256_storeu_pd(vecRes, mm);

    for (auto vecRe : vecRes) {
      res += vecRe;
    }
  }

  return res;
}
