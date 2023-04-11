// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_PERFORMANCE_STOPWATCH_H_
#define DXFEED_JNI_NATIVE_SDK_PERFORMANCE_STOPWATCH_H_

namespace dxfeed::perf {
  class Stopwatch {
    constexpr static const int64_t MillisPerSecond = 1000000; // 10 ^ 6
    constexpr static const int64_t NanosPerSecond = MillisPerSecond * 1000; // 10 ^ 9

    int64_t elapsed_;
    int64_t startTimeStamp_;
    bool isRunning_ = false;

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
}  // namespace dxfeed::perf

#endif // DXFEED_JNI_NATIVE_SDK_PERFORMANCE_STOPWATCH_H_