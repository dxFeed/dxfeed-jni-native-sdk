#include <iostream>
#include <memory>

#include "api/utils/Diagnostic.h"
#include "api/utils/TimeAndSaleFormatter.h"
#include "api/utils/TimeUtil.h"
#include "api/utils/StringUtils.hpp"

template<class T>
constexpr inline void hash_combine(std::size_t& seed, const T& v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<>
struct std::hash<TimeAndSale> {
  std::size_t operator()(const TimeAndSale& ptr) const noexcept {
    std::size_t h = 0;
    hash_combine(h, ptr.event_symbol);
    hash_combine(h, ptr.event_symbol);
    hash_combine(h, ptr.event_time);
    hash_combine(h, ptr.index);
    hash_combine(h, ptr.event_flags);
    hash_combine(h, ptr.time_nano_part);
    hash_combine(h, ptr.exchange_code);
    hash_combine(h, ptr.price);
    hash_combine(h, ptr.size);
    hash_combine(h, ptr.bid_price);
    hash_combine(h, ptr.ask_price);
    hash_combine(h, ptr.exchangeSaleConditions);
    hash_combine(h, ptr.flags);
    hash_combine(h, ptr.buyer);
    hash_combine(h, ptr.seller);
    return h;
  }
};

namespace dxfeed::perf {
  void Diagnostic::TimerCallback() {
    auto eventsPerSec = GetEventsPerSec();
    auto listenerCallsPerSec = GetListenerCallsPerSec();

    std::cout << "\n";
    std::cout << diagnosticHeader << "\n";
    std::cout << "----------------------------------------------" << "\n";
    std::cout << "  Events                   : " << eventsPerSec << " (per/sec)" << "\n";
    std::cout << "  Listener Calls           : " << listenerCallsPerSec << " (per/sec)" << "\n";
    std::cout << "  Average Number of Events : " << eventsPerSec / listenerCallsPerSec << "\n";
    std::cout << "  Running Time             : " << _runningDiff.elapsedInSeconds() << "\n";

    _timerDiff.restart();
  }

  int64_t Diagnostic::GetAndResetEventCounter() {
    return _eventCounter.exchange(0);
  }

  int64_t Diagnostic::GetAndResetListenerCounter() {
    return _listenerCounter.exchange(0);
  }

  double Diagnostic::GetEventsPerSec() {
    return static_cast<double>(GetAndResetEventCounter()) / _timerDiff.elapsedInSeconds();
  }

  double Diagnostic::GetListenerCallsPerSec() {
    return static_cast<double>(GetAndResetListenerCounter()) / _timerDiff.elapsedInSeconds();
  }

  Diagnostic::Diagnostic(int64_t measurementPeriodInSeconds):
    _timer(new Timer(this, &Diagnostic::TimerCallback, measurementPeriodInSeconds))
  {
    _timerDiff.restart();
    _runningDiff.restart();
  }

  void Diagnostic::AddEventCounter(int64_t value) {
    _eventCounter += value;
  }

  void Diagnostic::AddListenerCounter(int64_t value) {
    _listenerCounter += value;
  }

  Diagnostic::~Diagnostic() {
    _timer->dispose();
    delete _timer;
  }

  static std::atomic<std::size_t> _blackHoleHashCode = 0;

  void Diagnostic::operator()(const std::vector<TimeAndSale>& events) {
    auto size = static_cast<int64_t>(events.size());
    AddListenerCounter(1);
    AddEventCounter(size);
    for (int i = 0; i < size; ++i) {
      _blackHoleHashCode += std::hash<TimeAndSale>()(events[i]);
    }
  }

  inline int64_t getTimeStampInMillis() {
    const auto point = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(point.time_since_epoch()).count();
  }

  // todo: see DxFeed.Graal.Net.Events.Market.TimeAndSale.Time.set
  inline void setTime(TimeAndSale* e, int64_t value) {
    e->index =
      (static_cast<int64_t>(TimeUtil::GetSecondsFromTime(value)) << 32) |
      (static_cast<int64_t>(TimeUtil::GetMillisFromTime(value)) << 22) |
      (static_cast<uint32_t>(TimeAndSaleFormatter::getSequence(e)));
  }

  void Receiver::operator()(std::vector<TimeAndSale>& events) {
    for (auto& event: events) {
      _totalEventsCounter++;

      if (event.size == 100) {  // ---------------------  start mark ----------------------------
        auto current = getTimeStampInMillis();
        int64_t millis = TimeAndSaleFormatter::getTime(&event);
        _firstEventLatency = current - millis;
        setTime(&event, current);

        //publisher.publishEvents(Collections.singletonList(t));
      } else if (event.size == 300) { // ---------------------  end mark ----------------------------
        auto current = getTimeStampInMillis();
        int64_t millis = TimeAndSaleFormatter::getTime(&event);
        _lastEventLatency = current - millis;

        int64_t elapsed = _lastEventLatency - _firstEventLatency;
        double perEvent = static_cast<double>(elapsed) / _totalEventsCounter;
        int64_t speed = _totalEventsCounter / elapsed * 1000;
        setTime(&event, current);

        //publisher.publishEvents(Collections.singletonList(t));

        auto time = StringUtils::encodeTime<std::chrono::milliseconds>(current);
        printf("%s\t%lld\t%lld\t%lld\t%lld\t%.6f\t%lld\n",
               time.c_str(), _firstEventLatency, _lastEventLatency, elapsed, _totalEventsCounter, perEvent, speed);
        fflush(stdout);

        // reset variables
        _firstEventLatency = 0;
        _lastEventLatency = 0;
        _totalEventsCounter = 0;
      }
    }
  }
}