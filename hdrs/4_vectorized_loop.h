//
// Created by maletsden on 17.08.21.
//

#ifndef CACHE_OPTIMIZATION_4_VECTORIZED_LOOP_H
#define CACHE_OPTIMIZATION_4_VECTORIZED_LOOP_H

#include "types.h"

MATRIX_VAL vectorizedSSELoopSum(const MATRIX_VAL* data, size_t rows, size_t cols);
MATRIX_VAL vectorizedSSEUnrolledLoopSum(const MATRIX_VAL* data, size_t rows, size_t cols);
MATRIX_VAL vectorizedAVXLoopSum(const MATRIX_VAL* data, size_t rows, size_t cols);
MATRIX_VAL vectorizedAVXUnrollLoopSum(const MATRIX_VAL *data, size_t rows, size_t cols);

#endif //CACHE_OPTIMIZATION_4_VECTORIZED_LOOP_H
