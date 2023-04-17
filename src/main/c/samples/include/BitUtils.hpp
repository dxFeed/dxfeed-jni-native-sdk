// SPDX-License-Identifier: MPL-2.0

#ifndef DXFEED_JNI_NATIVE_SDK_BIT_UTILS_H_
#define DXFEED_JNI_NATIVE_SDK_BIT_UTILS_H_

#include <cstdint>

namespace dxfeed {
  struct BitUtils {
    /**
     * @brief Gets bits form flags.
     * @tparam T The flag and mask types.
     * @param[in] flags The bit flags.
     * @param[in] mask The bit mask.
     * @param[in] shift The bit right shift.
     * @return Returns bit value.
     */
    template<class T>
    inline static T getBits(T flags, T mask, uint32_t shift) {
      return (flags >> shift) & mask;
    }
  };
}  // namespace dxfeed

#endif // DXFEED_JNI_NATIVE_SDK_BIT_UTILS_H_
