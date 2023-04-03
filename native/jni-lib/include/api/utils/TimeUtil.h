#pragma once

#include <cstdint>
#include <algorithm>
#include <limits>

#include "MathUtil.h"

namespace dxfeed::perf {
  class TimeUtil {
    static const int64_t Second = 1000;
    static const int64_t Minute = 60 * Second;
    static const int64_t Hour = 60 * Minute;
    static const int64_t Day = 24 * Hour;
  public:
    static int32_t GetSecondsFromTime(int64_t timeMillis) {
      if (timeMillis >= 0) {
        auto value = static_cast<int32_t>(timeMillis / Second);
        return std::min(value, std::numeric_limits<int32_t>::max());
      } else {
        auto value = static_cast<int32_t>(((timeMillis + 1) / Second) - 1);
        return std::max(value, std::numeric_limits<int32_t>::min());
      }
    }

    static int GetMillisFromTime(int64_t timeMillis) {
      return static_cast<int32_t>(MathUtil::FloorMod(timeMillis, Second));
    }
  };
}