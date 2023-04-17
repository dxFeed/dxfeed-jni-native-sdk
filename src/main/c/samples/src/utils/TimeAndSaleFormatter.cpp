// SPDX-License-Identifier: MPL-2.0

#include <sstream>

#include "BitUtils.hpp"
#include "TimeAndSaleFormatter.hpp"
#include "StringUtils.hpp"

namespace dxfeed {

int64_t TimeAndSaleFormatter::getTime(const TimeAndSale *e) {
    return (e->index_ >> 32) * 1000 + ((e->index_ >> 22) & 0x3ff);
}

int32_t TimeAndSaleFormatter::getSequence(const TimeAndSale *e) {
    return static_cast<int32_t>(e->index_) & MAX_SEQUENCE;
}

int16_t TimeAndSaleFormatter::getTradeThroughExempt(const TimeAndSale *e) {
    return static_cast<int16_t>(BitUtils::getBits(e->flags_, TTE_MASK, TTE_SHIFT));
}

bool TimeAndSaleFormatter::isSpreadLeg(const TimeAndSale *e) {
    return (e->flags_ & SPREAD_LEG) != 0;
}

bool TimeAndSaleFormatter::isExtendedTradingHours(const TimeAndSale *e) {
    return (e->flags_ & ETH) != 0;
}

bool TimeAndSaleFormatter::isValidTick(const TimeAndSale *e) {
    return (e->flags_ & VALID_TICK) != 0;
}

std::string TimeAndSaleFormatter::toString(const TimeAndSale* e) {
    std::stringstream ss{};
    ss.precision(9);
    // clang-format off
    ss << "TimeAndSale{" << StringUtils::encodeString(e->eventSymbol_) <<
       ", eventTime=" << StringUtils::encodeTime<std::chrono::milliseconds>(e->eventTime_) <<
       ", eventFlags=0x" << std::hex << e->eventFlags_ << std::dec <<
       ", time=" << StringUtils::encodeTime<std::chrono::milliseconds>(getTime(e)) <<
       ", timeNanoPart=" << e->timeNanoPart_ <<
                                             ", sequence=" << getSequence(e) <<
                                                                             ", exchange=" << StringUtils::encodeUtf16Char(e->exchangeCode_) <<
                                                                             ", price_=" << e->price_ <<
                                                                             ", size=" << e->size_ <<
                                                                             ", bid=" << e->bidPrice_ <<
                                                                             ", ask=" << e->askPrice_ <<
                                                                             ", TTE=" << StringUtils::encodeUtf16Char(getTradeThroughExempt(e)) <<
                                                                             ", spread=" << StringUtils::encodeBool(isSpreadLeg(e)) <<
                                                                             ", ETH=" << StringUtils::encodeBool(isExtendedTradingHours(e)) <<
                                                                             ", validTick=" << StringUtils::encodeBool(isValidTick(e)) <<
                                                                             (e->buyer_.empty() ? "" : (", byuer='" + e->buyer_ + "'")) <<
                                                                             (e->seller_.empty() ? "" : (", seller_='" + e->seller_ + "'")) <<
                                                                             '}';
    // clang-format on
    return ss.str();
}

} // namespace dxfeed
