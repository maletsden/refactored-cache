//
// Created by maletsden on 16.08.21.
//

#include "../hdrs/1_colwise_read.h"

MATRIX_VAL colwiseReadSum(const MATRIX_VAL* data, size_t rows, size_t cols) {
  MATRIX_VAL res = MATRIX_VAL();

  for (auto row = 0; row < rows; ++row) {
    for (auto col = 0; col < cols; ++col) {
      res += data[col * cols + row];
    }
  }

  return res;
}