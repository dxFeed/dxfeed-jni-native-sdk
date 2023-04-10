#pragma once

#include <cstdint>

namespace dxfeed::perf {
  class MathUtil {
//    static const int64_t NegativeZeroBits = BitConverter.DoubleToInt64Bits(-0.0);

  public:
    static int32_t abs(int32_t a) {
      return (a < 0) ? -a : a;
    }

    static int32_t div(int32_t a, int32_t b) {
      if (a >= 0) {
        return a / b;
      }

      if (b >= 0) {
        return ((a + 1) / b) - 1;
      }

      return ((a + 1) / b) + 1;
    }

    static int64_t floorDiv(int64_t x, int64_t y) {
      auto r = x / y;

      // If the signs are different and modulo not zero, round down.
      if ((x ^ y) < 0 && r * y != x) {
        --r;
      }

      return r;
    }

    static int64_t floorMod(int64_t x, int64_t y) {
      return x - (floorDiv(x, y) * y);
    }

    static bool isPowerOfTwo(int64_t x) {
      return x > 0 && (x & (x - 1)) == 0;
    }

//    static bool IsNegativeZero(double x) {
//      BitConverter.DoubleToInt64Bits(x) == NegativeZeroBits;
//    }
  };
}
