// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_PERFORMANCE_TIME_UTIL_H_
#define DXFEED_JNI_NATIVE_SDK_PERFORMANCE_TIME_UTIL_H_

#include <cstdint>
#include <algorithm>
#include <limits>

#include "MathUtil.hpp"

namespace dxfeed::perf {
  struct TimeUtil {
    static int32_t getSecondsFromTime(int64_t timeMillis) {
      if (timeMillis >= 0) {
        auto value = static_cast<int32_t>(timeMillis / SECOND);
        return std::min(value, std::numeric_limits<int32_t>::max());
      } else {
        auto value = static_cast<int32_t>(((timeMillis + 1) / SECOND) - 1);
        return std::max(value, std::numeric_limits<int32_t>::min());
      }
    }

    static int getMillisFromTime(int64_t timeMillis) {
      return static_cast<int32_t>(MathUtil::floorMod(timeMillis, SECOND));
    }
  private:
    static const int64_t SECOND = 1000;
    static const int64_t MINUTE = 60 * SECOND;
    static const int64_t HOUR = 60 * MINUTE;
    static const int64_t DAY = 24 * HOUR;
  };
} // namespace dxfeed::perf

#endif // DXFEED_JNI_NATIVE_SDK_PERFORMANCE_TIME_UTIL_H_