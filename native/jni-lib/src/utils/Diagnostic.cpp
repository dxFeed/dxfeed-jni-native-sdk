#include <iostream>

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
    hash_combine(h, ptr.eventSymbol_);
    hash_combine(h, ptr.eventSymbol_);
    hash_combine(h, ptr.eventTime_);
    hash_combine(h, ptr.index_);
    hash_combine(h, ptr.eventFlags_);
    hash_combine(h, ptr.timeNanoPart_);
    hash_combine(h, ptr.exchangeCode_);
    hash_combine(h, ptr.price_);
    hash_combine(h, ptr.size_);
    hash_combine(h, ptr.bidPrice_);
    hash_combine(h, ptr.askPrice_);
    hash_combine(h, ptr.exchangeSaleConditions_);
    hash_combine(h, ptr.flags_);
    hash_combine(h, ptr.buyer_);
    hash_combine(h, ptr.seller_);
    return h;
  }
};

namespace dxfeed::perf {
  void Diagnostic::timerCallback() {
    auto eventsPerSec = getEventsPerSec();
    auto listenerCallsPerSec = getListenerCallsPerSec();

    std::cout << "\n";
    std::cout << "SystemInfo" << "\n";
    std::cout << "----------------------------------------------" << "\n";
    std::cout << "  Events                   : " << eventsPerSec << " (per/sec)" << "\n";
    std::cout << "  Listener Calls           : " << listenerCallsPerSec << " (per/sec)" << "\n";
    std::cout << "  Average Number of Events : " << eventsPerSec / listenerCallsPerSec << "\n";
    std::cout << "  Running Time             : " << runningDiff_.elapsedInSeconds() << "\n";

    timerDiff_.restart();
  }

  int64_t Diagnostic::getAndResetEventCounter() {
    return eventCounter_.exchange(0);
  }

  int64_t Diagnostic::getAndResetListenerCounter() {
    return listenerCounter_.exchange(0);
  }

  double Diagnostic::getEventsPerSec() {
    return static_cast<double>(getAndResetEventCounter()) / timerDiff_.elapsedInSeconds();
  }

  double Diagnostic::getListenerCallsPerSec() {
    return static_cast<double>(getAndResetListenerCounter()) / timerDiff_.elapsedInSeconds();
  }

  Diagnostic::Diagnostic(int64_t measurementPeriodInSeconds):
      timer_(new Timer(this, &Diagnostic::timerCallback, measurementPeriodInSeconds))
  {
    timerDiff_.restart();
    runningDiff_.restart();
  }

  void Diagnostic::addEventCounter(int64_t value) {
    eventCounter_ += value;
  }

  void Diagnostic::addListenerCounter(int64_t value) {
    listenerCounter_ += value;
  }

  Diagnostic::~Diagnostic() {
    timer_->dispose();
    delete timer_;
  }

  static std::atomic<std::size_t> _blackHoleHashCode = 0;

  void Diagnostic::operator()(const std::vector<TimeAndSale>& events) {
    auto size = static_cast<int64_t>(events.size());
    addListenerCounter(1);
    addEventCounter(size);
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
    e->index_ =
        (static_cast<int64_t>(TimeUtil::getSecondsFromTime(value)) << 32) |
        (static_cast<int64_t>(TimeUtil::getMillisFromTime(value)) << 22) |
        (static_cast<uint32_t>(TimeAndSaleFormatter::getSequence(e)));
  }

  void Receiver::operator()(std::vector<TimeAndSale>& events) {
    for (auto& event: events) {
      totalEventsCounter_++;

      if (event.size_ == 100) {  // ---------------------  start mark ----------------------------
        auto current = getTimeStampInMillis();
        int64_t millis = TimeAndSaleFormatter::getTime(&event);
        firstEventLatency_ = current - millis;
        setTime(&event, current);

        //publisher.publishEvents(Collections.singletonList(t));
      } else if (event.size_ == 300) { // ---------------------  end mark ----------------------------
        auto current = getTimeStampInMillis();
        int64_t millis = TimeAndSaleFormatter::getTime(&event);
        lastEventLatency_ = current - millis;

        int64_t elapsed = lastEventLatency_ - firstEventLatency_;
        double perEvent = static_cast<double>(elapsed) / totalEventsCounter_;
        int64_t speed = totalEventsCounter_ / elapsed * 1000;
        setTime(&event, current);

        //publisher.publishEvents(Collections.singletonList(t));

        auto time = StringUtils::encodeTime<std::chrono::milliseconds>(current);
        printf("%s\t%lld\t%lld\t%lld\t%lld\t%.6f\t%lld\n",
               time.c_str(), firstEventLatency_, lastEventLatency_, elapsed, totalEventsCounter_, perEvent, speed);
        fflush(stdout);

        // reset variables
        firstEventLatency_ = 0;
        lastEventLatency_ = 0;
        totalEventsCounter_ = 0;
      }
    }
  }
}