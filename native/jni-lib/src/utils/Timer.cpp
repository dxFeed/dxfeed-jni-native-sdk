#include "api/utils/Diagnostic.h"

namespace dxfeed::perf {
  Timer::Timer(Diagnostic* diagnostic, TimerCallback callback, int64_t measurementInSeconds):
    _diagnostic(diagnostic),
    m_func(callback),
    m_interval(measurementInSeconds)
  {
    start();
  }

  /**
 * Starting the timer.
 */
  void Timer::start() {
    m_running = true;
    m_thread = std::thread([&]() {
      while (m_running) {
        auto delta = std::chrono::steady_clock::now() + std::chrono::seconds(m_interval);
        (_diagnostic->*m_func)();
        std::this_thread::sleep_until(delta);
      }
    });
    m_thread.detach();
  }

  /*
   *  Stopping the timer and destroys the thread.
   */
  void Timer::stop() {
    m_running = false;
    m_thread.~thread();
  }

  /*
   *  Restarts the timer. Needed if you set a new
   *  timer interval for example.
   */
  void Timer::restart() {
    stop();
    start();
  }


  void Timer::dispose() {
    stop();
  }
}