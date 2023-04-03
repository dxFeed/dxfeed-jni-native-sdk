#pragma once

namespace dxfeed::perf {
  class Stopwatch {
    constexpr static const int64_t MillisPerSecond = 1000000; // 10 ^ 6
    constexpr static const int64_t NanosPerSecond = MillisPerSecond * 1000; // 10 ^ 9

    int64_t _elapsed;
    int64_t _startTimeStamp;
    bool _isRunning = false;

  public:
    explicit Stopwatch();

    void start();
    void stop();
    void reset();
    void restart();

    double elapsedInSeconds() const;
  private:
    static int64_t getTimeStampInNanos();
    int64_t getElapsedTimeInNanos() const;
  };
}


