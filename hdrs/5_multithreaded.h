//
// Created by maletsden on 17.08.21.
//

#ifndef CACHE_OPTIMIZATION_5_MULTITHREADED_H
#define CACHE_OPTIMIZATION_5_MULTITHREADED_H

#include "types.h"

MATRIX_VAL threadedAtomicReadSum(const MATRIX_VAL* data, size_t rows, size_t cols);
MATRIX_VAL threadedReadSum(const MATRIX_VAL* data, size_t rows, size_t cols);
MATRIX_VAL threadedCacheCoherencyReadSum(const MATRIX_VAL* data, size_t rows, size_t cols);

#endif //CACHE_OPTIMIZATION_5_MULTITHREADED_H
