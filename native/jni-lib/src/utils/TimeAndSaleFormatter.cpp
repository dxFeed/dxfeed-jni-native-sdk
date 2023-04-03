#include <sstream>

#include "api/utils/BitUtils.hpp"
#include "api/utils/TimeAndSaleFormatter.h"
#include "api/utils/StringUtils.hpp"

namespace dxfeed {

int64_t TimeAndSaleFormatter::getTime(const TimeAndSale *e) {
    return (e->index >> 32) * 1000 + ((e->index >> 22) & 0x3ff);
}

int32_t TimeAndSaleFormatter::getSequence(const TimeAndSale *e) {
    return static_cast<int32_t>(e->index) & MAX_SEQUENCE;
}

int16_t TimeAndSaleFormatter::getTradeThroughExempt(const TimeAndSale *e) {
    return static_cast<int16_t>(BitUtils::getBits(e->flags, TTE_MASK, TTE_SHIFT));
}

bool TimeAndSaleFormatter::isSpreadLeg(const TimeAndSale *e) {
    return (e->flags & SPREAD_LEG) != 0;
}

bool TimeAndSaleFormatter::isExtendedTradingHours(const TimeAndSale *e) {
    return (e->flags & ETH) != 0;
}

bool TimeAndSaleFormatter::isValidTick(const TimeAndSale *e) {
    return (e->flags & VALID_TICK) != 0;
}

std::string TimeAndSaleFormatter::toString(const TimeAndSale* e) {
    std::stringstream ss{};
    ss.precision(9);
    // clang-format off
    ss << "TimeAndSale{" <<  StringUtils::encodeString(e->event_symbol) <<
       ", eventTime=" << StringUtils::encodeTime<std::chrono::milliseconds>(e->event_time) <<
       ", eventFlags=0x" << std::hex << e->event_flags << std::dec <<
       ", time=" << StringUtils::encodeTime<std::chrono::milliseconds>(getTime(e)) <<
       ", timeNanoPart=" << e->time_nano_part <<
       ", sequence=" << getSequence(e) <<
       ", exchange=" << StringUtils::encodeUtf16Char(e->exchange_code) <<
       ", price=" << e->price <<
       ", size=" << e->size <<
       ", bid=" << e->bid_price <<
       ", ask=" << e->ask_price <<
       ", TTE=" << StringUtils::encodeUtf16Char(getTradeThroughExempt(e)) <<
       ", spread=" << StringUtils::encodeBool(isSpreadLeg(e)) <<
       ", ETH=" << StringUtils::encodeBool(isExtendedTradingHours(e)) <<
       ", validTick=" << StringUtils::encodeBool(isValidTick(e)) <<
       (e->buyer.empty() ? "" : (", byuer='" + e->buyer + "'")) <<
       (e->seller.empty() ? "" : (", seller='" + e->seller + "'")) <<
       '}';
    // clang-format on
    return ss.str();
}

} // namespace dxfg
