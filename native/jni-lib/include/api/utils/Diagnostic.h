#pragma once

#include <cstdint>
#include <thread>
#include <iostream>
#include <vector>

#include "StopWatch.h"
#include "api/TimeAndSale.h"

namespace dxfeed::perf {
  class Diagnostic;
  typedef void(Diagnostic::*TimerCallback)();
  void setProcessPriorityClass();

  struct Timer {
    Timer(Diagnostic* diagnostic, TimerCallback callback, int64_t measurementInSeconds);
    void start();
    void restart();
    void stop();
    void dispose();
  private:
    Diagnostic* _diagnostic;
    const TimerCallback m_func;   // Function to be executed after interval
    const int64_t m_interval;     // Timer interval in milliseconds
    std::thread m_thread;         // Thread timer is running into
    bool m_running = false;       // Status if timer is running
  };

  class Diagnostic {
    const std::string diagnosticHeader = "SystemInfo";
    /* readonly Timer */  Timer* _timer;

    /* readonly Stopwatch */ Stopwatch _timerDiff;   // todo: replace TimerDiff with Stopwatch
    /* readonly Stopwatch */ Stopwatch _runningDiff; // todo: replace TimerDiff with Stopwatch

    /* long */ std::atomic<int64_t> _eventCounter {0};
    /* long */ std::atomic<int64_t> _listenerCounter {0};

    void TimerCallback();
    int64_t GetAndResetEventCounter();
    int64_t GetAndResetListenerCounter();
    double GetEventsPerSec();
    double GetListenerCallsPerSec();
  public:
    explicit Diagnostic(int64_t measurementPeriodInSeconds);
    ~Diagnostic();
    void operator()(const std::vector<TimeAndSale>& events);
    void AddEventCounter(int64_t value);
    void AddListenerCounter(int64_t value);
  };

  struct Receiver {
    void operator()(std::vector<TimeAndSale>& events);
  private:
    int64_t _firstEventLatency = 0;
    int64_t _lastEventLatency = 0;
    int64_t _totalEventsCounter = 0;
  };
}