#pragma once

#include <cstdint>
#include <thread>
#include <iostream>
#include <vector>

#include "StopWatch.h"
#include "api/TimeAndSale.h"

namespace dxfeed::perf {
  struct Diagnostic;
  typedef void(Diagnostic::*TimerCallback)();
  void setProcessPriorityClass();

  struct Timer {
    Timer(Diagnostic* diagnostic, TimerCallback callback, int64_t measurementInSeconds);
    void start();
    void stop();
    void dispose();
  private:
    Diagnostic* diagnostic_;
    const TimerCallback callback_;   // Function to be executed after interval
    const int64_t timeInterval_;     // Timer interval in milliseconds
    std::thread thread_;         // Thread timer is running into
    bool isRunning_ = false;       // Status if timer is running
  };

  struct Diagnostic {
    explicit Diagnostic(int64_t measurementPeriodInSeconds);
    ~Diagnostic();
    void operator()(const std::vector<TimeAndSale>& events);
    void addEventCounter(int64_t value);
    void addListenerCounter(int64_t value);
  private:
    Timer* timer_;
    Stopwatch timerDiff_;
    Stopwatch runningDiff_;
    std::atomic<int64_t> eventCounter_ {0};
    std::atomic<int64_t> listenerCounter_ {0};

    void timerCallback();
    int64_t getAndResetEventCounter();
    int64_t getAndResetListenerCounter();
    double getEventsPerSec();
    double getListenerCallsPerSec();
  };

  struct Receiver {
    void operator()(std::vector<TimeAndSale>& events);
  private:
    int64_t firstEventLatency_ = 0;
    int64_t lastEventLatency_ = 0;
    int64_t totalEventsCounter_ = 0;
  };
}