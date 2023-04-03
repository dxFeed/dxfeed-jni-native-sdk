#pragma once

#include <string>

struct TimeAndSale {
  std::string event_symbol;
  int64_t event_time;
  int64_t index;
  int64_t event_flags;
  int32_t time_nano_part;
  int8_t exchange_code;
  double price;
  int64_t size;
  double bid_price;
  double ask_price;
  std::string exchangeSaleConditions;
  int32_t flags;
  std::string buyer;
  std::string seller;
};