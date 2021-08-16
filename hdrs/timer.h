//
// Created by maletsden on 16.08.21.
//

#ifndef CACHE_OPTIMIZATION_TIMER_H
#define CACHE_OPTIMIZATION_TIMER_H

#include <chrono>
#include <atomic>
#include <type_traits>

namespace timer {
  inline auto getCurrentTimeFenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    const auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
  }

  template<class D>
  inline auto toUs(D&& d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
  }

  using TIME_DIFF_TYPE [[maybe_unused]] = decltype(getCurrentTimeFenced() - getCurrentTimeFenced());
  using TIME_TYPE = decltype(toUs(TIME_DIFF_TYPE()));

}
#endif //CACHE_OPTIMIZATION_TIMER_H
