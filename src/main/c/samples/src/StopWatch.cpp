// SPDX-License-Identifier: MPL-2.0

#include <cstdint>
#include <chrono>

#include "StopWatch.hpp"

namespace dxfeed::perf {

  Stopwatch::Stopwatch() :
      elapsed_(0),
      startTimeStamp_(0),
      isRunning_(false)
  {
    reset();
  }

  void Stopwatch::start() {
    // Calling start on a running Stopwatch is a no-op.
    if (!isRunning_) {
      startTimeStamp_ = getTimeStampInNanos();
      isRunning_ = true;
    }
  }

  void Stopwatch::stop() {
    // Calling stop on a stopped Stopwatch is a no-op.
    if (isRunning_) {
      int64_t endTimeStamp = getTimeStampInNanos();
      int64_t elapsedThisPeriod = endTimeStamp - startTimeStamp_;
      elapsed_ += elapsedThisPeriod;
      isRunning_ = false;

      if (elapsed_ < 0) {
        // When measuring small time periods the Stopwatch.Elapsed*
        // properties can return negative values.  This is due to
        // bugs in the basic input/output system (BIOS) or the hardware
        // abstraction layer (HAL) on machines with variable-speed CPUs
        // (e.g. Intel SpeedStep).

        elapsed_ = 0;
      }
    }
  }

  void Stopwatch::reset() {
    elapsed_ = 0;
    isRunning_ = false;
    startTimeStamp_ = 0;
  }

  // Convenience method for replacing {sw.Reset(); sw.Start();} with a single sw.Restart()
  void Stopwatch::restart() {
    elapsed_ = 0;
    startTimeStamp_ = getTimeStampInNanos();
    isRunning_ = true;
  }

  double Stopwatch::elapsedInSeconds() const {
    return static_cast<double>(getElapsedTimeInNanos()) / static_cast<double>(NanosPerSecond);
  }

  int64_t Stopwatch::getTimeStampInNanos() {
    const auto point = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(point.time_since_epoch()).count();
  }

  // Get the elapsed ticks.
  int64_t Stopwatch::getElapsedTimeInNanos() const {
    int64_t timeElapsed = elapsed_;

    if (isRunning_) {
      // If the Stopwatch is running, add elapsed time since the Stopwatch is started last time.
      int64_t currentTimeStamp = getTimeStampInNanos();
      int64_t elapsedUntilNow = currentTimeStamp - startTimeStamp_;
      timeElapsed += elapsedUntilNow;
    }
    return timeElapsed;
  }
}