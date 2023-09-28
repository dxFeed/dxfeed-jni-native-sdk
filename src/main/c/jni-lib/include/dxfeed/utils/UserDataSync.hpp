// SPDX-License-Identifier: MPL-2.0

#ifndef NATIVE_JNI_SDK_USERDATASYNC_HPP
#define NATIVE_JNI_SDK_USERDATASYNC_HPP

#include <mutex>
#include <vector>
#include <condition_variable>

namespace dxfeed::user_data_sync {
    extern std::mutex LOCK;
    extern std::condition_variable CONDITION_VAR;
    extern std::atomic_bool CONSUMER_PROCESSED_DATA;
    extern jlong GLOBAL_JAVA_USER_CALLBACK_ADDRESS;
    extern jlong GLOBAL_JAVA_USER_DATA_ADDRESS;
    extern std::vector<char> GLOBAL_BYTE_ARRAY;
    extern std::vector<double> GLOBAL_DOUBLE_ARRAY;
    extern std::vector<char> GLOBAL_EVENT_TYPE_ARRAY;
  }

#endif //NATIVE_JNI_SDK_USERDATASYNC_HPP
