// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_TIME_AND_SALE_H_
#define DXFEED_JNI_NATIVE_SDK_TIME_AND_SALE_H_

// todo: replace string with const char*
#include <string>

struct TimeAndSale {
  std::string eventSymbol_;
  int64_t eventTime_;
  int64_t index_;
  int64_t eventFlags_;
  int32_t timeNanoPart_;
  int8_t exchangeCode_;
  double price_;
  int64_t size_;
  double bidPrice_;
  double askPrice_;
  std::string exchangeSaleConditions_;
  int32_t flags_;
  std::string buyer_;
  std::string seller_;
};

#endif // DXFEED_JNI_NATIVE_SDK_TIME_AND_SALE_H_