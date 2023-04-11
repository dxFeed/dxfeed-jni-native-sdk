// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_TIME_FORMATTER_H_
#define DXFEED_JNI_NATIVE_SDK_TIME_FORMATTER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

#include "../TimeAndSale.h"

namespace dxfeed {

class TimeAndSaleFormatter {
  private:
    static const int32_t MAX_SEQUENCE = (1 << 22) - 1;

    /*
     * Flags property has several significant bits that are packed into an integer in the following way:
     *   31..16   15...8    7    6    5    4    3    2    1    0
     * +--------+--------+----+----+----+----+----+----+----+----+
     * |        |   TTE  |    |  Side   | SL | ETH| VT |  Type   |
     * +--------+--------+----+----+----+----+----+----+----+----+
     */

    // TTE (TradeThroughExempt) values are ASCII chars in [0, 255].
    static const int32_t TTE_MASK = 0xff;
    static const int32_t TTE_SHIFT = 8;

    // SIDE values are taken from Side enum.
    static const int32_t SIDE_MASK = 3;
    static const int32_t SIDE_SHIFT = 5;

    static const int32_t SPREAD_LEG = 1 << 4;
    static const int32_t ETH = 1 << 3;
    static const int32_t VALID_TICK = 1 << 2;

    // TYPE values are taken from TimeAndSaleType enum.
    static const int32_t TYPE_MASK = 3;
    static const int32_t TYPE_SHIFT = 0;

    static const std::unordered_map<TimeAndSale, std::string> typesMap_;

  public:
    static int64_t getTime(const TimeAndSale *e);
    static int32_t getSequence(const TimeAndSale *e);
    static int16_t getTradeThroughExempt(const TimeAndSale *e);
    static bool isSpreadLeg(const TimeAndSale *e);
    static bool isExtendedTradingHours(const TimeAndSale *e);
    static bool isValidTick(const TimeAndSale *e);
    static TimeAndSale getType(const TimeAndSale *e);
    static std::string toString(const TimeAndSale *e);
};

} // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_TIME_FORMATTER_H_