//
// Created by maletsden on 17.08.21.
//

#include "../hdrs/3_unroll_loop.h"

MATRIX_VAL unrollLoopSum(const MATRIX_VAL* data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col + 5 <= cols; col += 5) {
      res += data[rowI + col];
      res += data[rowI + col + 1];
      res += data[rowI + col + 2];
      res += data[rowI + col + 3];
      res += data[rowI + col + 4];
    }

    for (auto col = cols - (cols % 5); col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  return res;
}