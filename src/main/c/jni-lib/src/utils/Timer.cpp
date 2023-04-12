// SPDX-License-Identifier: MPL-2.0

#include "dxfeed/utils/Diagnostic.hpp"

namespace dxfeed::perf {
  Timer::Timer(Diagnostic* diagnostic, TimerCallback callback, int64_t measurementInSeconds):
      diagnostic_(diagnostic),
      callback_(callback),
      timeInterval_(measurementInSeconds)
  {
    start();
  }

  /**
 * Starting the timer.
 */
  void Timer::start() {
    isRunning_ = true;
    thread_ = std::thread([&]() {
      while (isRunning_) {
        auto delta = std::chrono::steady_clock::now() + std::chrono::seconds(timeInterval_);
        (diagnostic_->*callback_)();
        std::this_thread::sleep_until(delta);
      }
    });
    thread_.detach();
  }

  /*
   *  Stopping the timer and destroys the thread.
   */
  void Timer::stop() {
    isRunning_ = false;
    thread_.~thread();
  }

  void Timer::dispose() {
    stop();
  }
}