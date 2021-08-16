//
// Created by maletsden on 17.08.21.
//

#include "../hdrs/2_rowwise_read.h"

MATRIX_VAL rowwiseReadSum(const MATRIX_VAL* data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  for (auto row = 0; row < rows; ++row) {
    const auto rowI = row * cols;
    for (auto col = 0; col < cols; ++col) {
      res += data[rowI + col];
    }
  }

  return res;
}