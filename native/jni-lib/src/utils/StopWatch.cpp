#include <cstdint>
#include <chrono>

#include "api/utils/StopWatch.h"

namespace dxfeed::perf {

  Stopwatch::Stopwatch() :
    _elapsed(0),
    _startTimeStamp(0),
    _isRunning(false)
  {
    reset();
  }

  void Stopwatch::start() {
    // Calling start on a running Stopwatch is a no-op.
    if (!_isRunning) {
      _startTimeStamp = getTimeStampInNanos();
      _isRunning = true;
    }
  }

  void Stopwatch::stop() {
    // Calling stop on a stopped Stopwatch is a no-op.
    if (_isRunning) {
      int64_t endTimeStamp = getTimeStampInNanos();
      int64_t elapsedThisPeriod = endTimeStamp - _startTimeStamp;
      _elapsed += elapsedThisPeriod;
      _isRunning = false;

      if (_elapsed < 0) {
        // When measuring small time periods the Stopwatch.Elapsed*
        // properties can return negative values.  This is due to
        // bugs in the basic input/output system (BIOS) or the hardware
        // abstraction layer (HAL) on machines with variable-speed CPUs
        // (e.g. Intel SpeedStep).

        _elapsed = 0;
      }
    }
  }

  void Stopwatch::reset() {
    _elapsed = 0;
    _isRunning = false;
    _startTimeStamp = 0;
  }

  // Convenience method for replacing {sw.Reset(); sw.Start();} with a single sw.Restart()
  void Stopwatch::restart() {
    _elapsed = 0;
    _startTimeStamp = getTimeStampInNanos();
    _isRunning = true;
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
    int64_t timeElapsed = _elapsed;

    if (_isRunning) {
      // If the Stopwatch is running, add elapsed time since the Stopwatch is started last time.
      int64_t currentTimeStamp = getTimeStampInNanos();
      int64_t elapsedUntilNow = currentTimeStamp - _startTimeStamp;
      timeElapsed += elapsedUntilNow;
    }
    return timeElapsed;
  }
}